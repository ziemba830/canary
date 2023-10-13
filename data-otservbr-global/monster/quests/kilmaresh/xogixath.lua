local mType = Game.createMonsterType("Xogixath")
local monster = {}

monster.description = "xogixath"
monster.experience = 22000
monster.outfit = {
	lookType = 842,
	lookHead = 3,
	lookBody = 16,
	lookLegs = 75,
	lookFeet = 79,
	lookAddons = 2,
	lookMount = 0,
}

monster.health = 28000
monster.maxHealth = 28000
monster.race = "fire"
monster.corpse = 12838
monster.speed = 95
monster.manaCost = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10,
}

monster.bosstiary = {
	bossRaceId = 1827,
	bossRace = RARITY_BANE,
}

monster.strategiesTarget = {
	nearest = 70,
	health = 10,
	damage = 10,
	random = 10,
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	rewardBoss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 90,
	targetDistance = 1,
	runHealth = 0,
	healthHidden = false,
	isBlockable = false,
	canWalkOnEnergy = true,
	canWalkOnFire = true,
	canWalkOnPoison = true,
}

monster.light = {
	level = 0,
	color = 0,
}

monster.voices = {
	interval = 5000,
	chance = 10,
}

monster.loot = {
	{ name = "platinum coin", chance = 100000, maxCount = 9 },
	{ id = 3039, chance = 91670, maxCount = 2 }, -- red gem
	{ name = "green crystal shard", chance = 19000 },
	{ name = "green crystal fragment", chance = 2700 },
	{ name = "wand of draconia", chance = 2700 },
	{ name = "blue crystal splinter", chance = 13510, maxCount = 5 },
	{ name = "sea horse figurine", chance = 3000 },
	{ name = "golden mask", chance = 5600 },
	{ name = "small sapphire", chance = 48000, maxCount = 3 },
	{ name = "small enchanted sapphire", chance = 16220, maxCount = 4 },
	{ name = "wand of inferno", chance = 2700 },
	{ name = "stone skin amulet", chance = 11000 },
	{ name = "fire axe", chance = 5000 },
	{ id = 31557, chance = 250 }, -- blister ring
	{ name = "winged boots", chance = 250 },
	{ id = 30401, chance = 750 }, -- amulet of theurgy
}

monster.attacks = {
	{ name = "melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -600 },
	{ name = "combat", interval = 2700, chance = 25, type = COMBAT_FIREDAMAGE, minDamage = -400, maxDamage = -550, radius = 4, effect = CONST_ME_FIREAREA, target = false },
	{ name = "combat", interval = 3100, chance = 30, type = COMBAT_DEATHDAMAGE, minDamage = -450, maxDamage = -666, range = 7, radius = 3, shootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA, target = true },
	{ name = "combat", interval = 3700, chance = 35, type = COMBAT_FIREDAMAGE, minDamage = -450, maxDamage = 600, range = 7, radius = 1, shootEffect = CONST_ANI_FIRE, effect = CONST_ME_EXPLOSIONHIT, target = true },
	{ name = "combat", interval = 4300, chance = 40, type = COMBAT_DEATHDAMAGE, minDamage = -400, maxDamage = -550, length = 7, spread = 3, effect = CONST_ME_MORTAREA, target = false },
}

monster.defenses = {
	defense = 86,
	armor = 86,
}

monster.elements = {
	{ type = COMBAT_PHYSICALDAMAGE, percent = 0 },
	{ type = COMBAT_ENERGYDAMAGE, percent = 0 },
	{ type = COMBAT_EARTHDAMAGE, percent = 0 },
	{ type = COMBAT_FIREDAMAGE, percent = 0 },
	{ type = COMBAT_LIFEDRAIN, percent = 0 },
	{ type = COMBAT_MANADRAIN, percent = 0 },
	{ type = COMBAT_DROWNDAMAGE, percent = 0 },
	{ type = COMBAT_ICEDAMAGE, percent = -10 },
	{ type = COMBAT_HOLYDAMAGE, percent = 0 },
	{ type = COMBAT_DEATHDAMAGE, percent = 50 },
}

monster.immunities = {
	{ type = "paralyze", condition = true },
	{ type = "outfit", condition = false },
	{ type = "invisible", condition = true },
	{ type = "bleed", condition = false },
}

mType.onThink = function(monster, interval) end

mType.onAppear = function(monster, creature)
	if monster:getType():isRewardBoss() then
		monster:setReward(true)
	end
end

mType.onDisappear = function(monster, creature) end

mType.onMove = function(monster, creature, fromPosition, toPosition) end

mType.onSay = function(monster, creature, type, message) end

mType:register(monster)
