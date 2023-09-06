#include <utility>

/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2022 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */
#pragma once

struct LuaNil {
} lua_nil;

class LuaField;
class LuaClass;
class LuaMethod;

template <typename T>
concept RegisterWithLua = requires(T t) {
	{ t.registerWithLua() } -> std::same_as<void>;
};

class LuaState {
public:
	lua_State* L;

	explicit LuaState(lua_State* state) :
		L(state) { }

	LuaState &operator<<(const LuaNil);
	LuaState &operator<<(const bool b);
	LuaState &operator<<(const int i);
	LuaState &operator<<(const lua_Number i);
	LuaState &operator<<(const char* s);
	LuaState &operator<<(const std::string &s);
	LuaState &operator<<(const lua_CFunction &func);

	template <RegisterWithLua T>
	LuaState &operator<<(const T &registerable);

	template <typename T>
	LuaState &operator<<(T* obj);

	template <typename T>
	LuaState &operator<<(const std::vector<T> &arr);

	template <typename K, typename V>
	LuaState &operator<<(const std::map<K, V> &mapData);

	LuaState &operator>>(const LuaNil);
	LuaState &operator>>(bool &b);
	LuaState &operator>>(std::string &str);

	template <typename T>
		requires std::integral<T> || std::floating_point<T>
	LuaState &operator>>(T &number);

	template <class T>
	LuaState &operator>>(T*&obj);

	LuaField operator[](const std::string index);
};

class LuaField {
private:
	LuaState &lua;
	const std::string index;
	const int8_t stackIndex;

public:
	LuaField(
		LuaState &L,
		const std::string idx,
		const int8_t stackIdx
	) :
		lua(L),
		index(idx), stackIndex(stackIdx) { }
	LuaField(LuaState &L, const std::string idx) :
		lua(L), index(idx), stackIndex(-2) { }

	template <typename T>
	LuaField &operator=(const T &value) {
		lua << value;
		lua_setfield(lua.L, stackIndex, index.c_str());
		return *this;
	}
};

class LuaClass {
private:
	LuaState &lua;
	const std::string className;
	const std::string baseClass;
	const lua_CFunction constructor = nullptr;

public:
	LuaClass(
		LuaState &L,
		std::string name
	) :
		lua(L),
		className(std::move(name)) { }

	LuaClass(
		LuaState &L,
		std::string name,
		std::string base
	) :
		lua(L),
		className(std::move(name)), baseClass(std::move(base)) { }

	LuaClass(
		LuaState &L,
		std::string name,
		const lua_CFunction constructorFunction
	) :
		lua(L),
		className(std::move(name)), constructor(constructorFunction) { }

	LuaClass(
		LuaState &L,
		std::string name,
		std::string base,
		const lua_CFunction constructorFunction
	) :
		lua(L),
		className(std::move(name)), baseClass(std::move(base)) { }

	void registerWithLua() const {
		lua_State* L = lua.L;

		// className = {}
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setglobal(L, className.c_str());
		int methods = lua_gettop(L);

		// methodsTable = {}
		lua_newtable(L);
		int methodsTable = lua_gettop(L);

		if (constructor) {
			lua << constructor;
			lua_setfield(L, methodsTable, "__call");
		}

		uint32_t parents = 0;
		if (!baseClass.empty()) {
			lua_getglobal(L, baseClass.c_str());
			lua_rawgeti(L, -1, 'p');
			parents = static_cast<uint32_t>(lua_tonumber(L, -1));
			lua_pop(L, 1);
			lua_setfield(L, methodsTable, "__index");
		}

		lua_setmetatable(L, methods);

		luaL_newmetatable(L, className.c_str());
		int metatable = lua_gettop(L);

		lua_pushvalue(L, methods);
		lua_setfield(L, metatable, "__metatable");

		lua_pushvalue(L, methods);
		lua_setfield(L, metatable, "__index");

		lua_pushnumber(L, std::hash<std::string>()(className));
		lua_rawseti(L, metatable, 'h');

		lua_pushnumber(L, parents);
		lua_rawseti(L, metatable, 'p');

		// Assume LuaData_t is an enum and you've included the magic_enum library
		auto userTypeEnum = magic_enum::enum_cast<LuaData_t>(className);
		if (userTypeEnum.has_value()) {
			lua_pushnumber(L, static_cast<lua_Number>(userTypeEnum.value()));
		} else {
			lua_pushnumber(L, static_cast<lua_Number>(LuaData_t::Unknown));
		}
		lua_rawseti(L, metatable, 't');

		lua_pop(L, 2);
	}
};

class LuaMethod {
private:
	LuaState &lua;
	const std::string className;
	const std::string methodName;
	const lua_CFunction func;
	const bool metaMethod = false;

public:
	LuaMethod(
		LuaState &L,
		std::string name,
		std::string method,
		const lua_CFunction func,
		const bool meta = false
	) :
		lua(L),
		className(std::move(name)), methodName(std::move(method)), func(func), metaMethod(meta) { }

	void registerWithLua() const {
		if (metaMethod) {
			luaL_getmetatable(lua.L, className.c_str());
		} else {
			lua_getglobal(lua.L, className.c_str());
		}

		lua[methodName] = func;

		lua_pop(lua.L, 1);
	}
};
