local teste = TalkAction("/beats")
local relogCache = {}

function mtime()
    return os.time()
end

function teste.onSay(player, words, param)
	local accId = player:getAccountId()
    	if not relogCache[accId] then
    		relogCache[accId] = 0
    	end

    	-- prevent spam
    	if mtime() < relogCache[accId] then
    		return false
    	end
    	relogCache[accId] = mtime() + 2

    	-- try to relog
    	local relog = player:fastRelog(player:getName())
    	if relog == RETURNVALUE_YOUCANNOTLOGOUTHERE then
    		player:sendColorMessage("You may not logout here!", MESSAGE_COLOR_PURPLE)
    	elseif relog == RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT then
    		player:sendColorMessage("You may not logout during or immediately after a fight!", MESSAGE_COLOR_PURPLE)
    	end

    	return false
end

teste:separator(" ")
teste:groupType("normal")
teste:register()
