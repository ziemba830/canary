/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2022 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#include "pch.hpp"
#include "lib/lua/lua_state.hpp"

LuaState &LuaState::operator<<(const LuaNil) {
	lua_pushnil(L);
	return *this;
}

LuaState &LuaState::operator<<(const bool b) {
	lua_pushboolean(L, (int) b);
	return *this;
}

LuaState &LuaState::operator<<(const int i) {
	lua_pushinteger(L, i);
	return *this;
}

LuaState &LuaState::operator<<(const lua_Number i) {
	lua_pushnumber(L, i);
	return *this;
}

LuaState &LuaState::operator<<(const char *s) {
	lua_pushstring(L, s);
	return *this;
}

LuaState &LuaState::operator<<(const std::string &s) {
	return *this << s.c_str();
}

LuaState &LuaState::operator<<(const lua_CFunction &func) {
	lua_pushcfunction(L, func);
	return *this;
}

template<RegisterWithLua T>
LuaState &LuaState::operator<<(const T &registerable) {
	registerable.registerWithLua();
	return *this;
}

template <typename T>
LuaState &LuaState::operator<<(T* obj) {
	T** userData = (T**) lua_newuserdata(L, sizeof(T*));
	*userData = obj;
	luaL_getmetatable(L, T::className);
	lua_setmetatable(L, -2);
	return *this;
}

template <typename T>
LuaState &LuaState::operator<<(const std::vector<T>& arr) {
	lua_newtable(L);
	for (size_t i = 0; i < arr.size(); ++i) {
		*this << arr[i];
		lua_rawseti(L, -2, i + 1);
	}
	return *this;
}

template <typename K, typename V>
LuaState& LuaState::operator<<(const std::map<K, V> &mapData) {
	lua_newtable(L);
	for (auto &[key, value] : mapData) {
		*this << key;
		*this << value;
		lua_settable(L, -3);
	}
	return *this;
}

LuaState &LuaState::operator>>(const LuaNil) {
	lua_pop(L, 1);
	return *this;
}

LuaState &LuaState::operator>>(bool &b) {
	b = lua_toboolean(L, -1) != 0;
	return *this >> lua_nil;
}

template <typename T>
requires std::integral<T> || std::floating_point<T>
LuaState &LuaState::operator>>(T &number) {
	number = static_cast<T>(lua_tonumber(L, -1));
	return *this >> lua_nil;
}

LuaState &LuaState::operator>>(std::string &str) {
	size_t len;
	const char* c_str = lua_tolstring(L, -1, &len);

	if (c_str && len > 0) {
		str = std::string(c_str, len);
	}

	return *this >> lua_nil;
}

template <class T>
LuaState &LuaState::operator>>(T *&obj) {
	T** userdata = (T**) lua_touserdata(L, -1);

	if (userdata) {
		obj = *userdata;
	}

	return *this >> lua_nil;
}

LuaField LuaState::operator[](const std::string index) {
	return LuaField(*this, index);
}