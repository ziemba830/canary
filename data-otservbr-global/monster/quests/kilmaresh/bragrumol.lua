local mType = Game.createMonsterType("Bragrumol")
local monster = {}

monster.description = "Bragrumol"
monster.experience = 18000
monster.outfit = {
	lookType = 856,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0,
}

monster.health = 38000
monster.maxHealth = 38000
monster.race = "fire"
monster.corpse = 12838
monster.speed = 110
monster.manaCost = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10,
}

monster.bosstiary = {
	bossRaceId = 1828,
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
	{ name = "flaming arrow", chance = 60000, maxCount = 15 },
	{ id = 3039, chance = 81250 }, -- red gem
	{ name = "sea horse figurine", chance = 6700 },
	{ name = "golden mask", chance = 12500 },
	{ id = 31557, chance = 250 }, -- blister ring
	{ name = "winged boots", chance = 750 },
	{ id = 30403, chance = 250 }, -- enchanted theurgic amulet
	{ name = "magma coat", chance = 5000 },
	{ name = "stone skin amulet", chance = 10000 },
	{ id = 281, chance = 28000 }, -- giant shimmering pearl (green)
}

monster.attacks = {
	{ name = "melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -600 },
	{ name = "combat", interval = 2700, chance = 20, type = COMBAT_DEATHDAMAGE, minDamage = -300, maxDamage = -600, range = 1, shootEffect = CONST_ANI_SUDDENDEATH, target = false },
	{ name = "combat", interval = 3100, chance = 25, type = COMBAT_DEATHDAMAGE, minDamage = -300, maxDamage = -600, radius = 4, effect = CONST_ME_MORTAREA, target = false },
	{ name = "combat", interval = 3700, chance = 30, type = COMBAT_FIREDAMAGE, minDamage = -300, maxDamage = -600, range = 5, radius = 4, shootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA, target = true },
	{ name = "energy chain", interval = 4300, chance = 40, minDamage = -350, maxDamage = -575, range = 3, target = true },
}

monster.defenses = {
	defense = 84,
	armor = 84,
}

monster.elements = {
	{ type = COMBAT_PHYSICALDAMAGE, percent = 0 },
	{ type = COMBAT_ENERGYDAMAGE, percent = 0 },
	{ type = COMBAT_EARTHDAMAGE, percent = 0 },
	{ type = COMBAT_FIREDAMAGE, percent = 30 },
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
