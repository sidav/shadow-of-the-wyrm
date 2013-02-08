#pragma once

enum ClassIdentifier
{
  CLASS_ID_NULL = 0,
  CLASS_ID_GAME,
  CLASS_ID_CURSES_DISPLAY,
  CLASS_ID_COMMAND_FACTORY,
  CLASS_ID_EQUIPMENT_COMMAND_FACTORY,
  CLASS_ID_INVENTORY_COMMAND_FACTORY,
  CLASS_ID_TILE_SELECTION_COMMAND_FACTORY,
  CLASS_ID_ACTION_COORDINATOR,
  CLASS_ID_ACTION_COST,
  CLASS_ID_KEYBOARD_COMMAND_MAP,
  CLASS_ID_EQUIPMENT_KEYBOARD_COMMAND_MAP,
  CLASS_ID_INVENTORY_KEYBOARD_COMMAND_MAP,
  CLASS_ID_TILE_SELECTION_KEYBOARD_COMMAND_MAP,
  CLASS_ID_ACTION_MANAGER,
  CLASS_ID_TILE_SELECTION_MANAGER,
  CLASS_ID_WORLD,
  CLASS_ID_CALENDAR,
  CLASS_ID_SPRING,
  CLASS_ID_SUMMER,
  CLASS_ID_AUTUMN,
  CLASS_ID_WINTER,
  CLASS_ID_WORLD_TIME_KEEPER,
  CLASS_ID_TIME_OBSERVER,
  CLASS_ID_CREATURE_TIME_OBSERVER,
  CLASS_ID_SEASONS_TIME_OBSERVER,
  CLASS_ID_MAP_REGISTRY,
  CLASS_ID_MAP,
  CLASS_ID_DIMENSIONS,
  CLASS_ID_DEPTH,
  CLASS_ID_MAP_EXIT,
  CLASS_ID_TILE,
  CLASS_ID_BARRACKS_TILE,
  CLASS_ID_BEACH_TILE,
  CLASS_ID_BUSH_TILE,
  CLASS_ID_CAIRN_TILE,
  CLASS_ID_CASTLE_TILE,
  CLASS_ID_CAVERN_TILE,
  CLASS_ID_CHURCH_TILE,
  CLASS_ID_DAIS_TILE,
  CLASS_ID_DESERT_TILE,
  CLASS_ID_DOWN_STAIRCASE_TILE,
  CLASS_ID_DUNGEON_COMPLEX_TILE,
  CLASS_ID_DUNGEON_TILE,
  CLASS_ID_FIELD_TILE,
  CLASS_ID_FOREST_TILE,
  CLASS_ID_GRAVE_TILE,
  CLASS_ID_BARROW_TILE,
  CLASS_ID_GRAVEYARD_TILE,
  CLASS_ID_HILLS_TILE,
  CLASS_ID_KEEP_TILE,
  CLASS_ID_LIBRARY_TILE,
  CLASS_ID_MARSH_TILE,
  CLASS_ID_MOUNTAINS_TILE,
  CLASS_ID_PIER_TILE,
  CLASS_ID_REEDS_TILE,
  CLASS_ID_RIVER_TILE,
  CLASS_ID_ROAD_TILE,
  CLASS_ID_ROCK_TILE,
  CLASS_ID_SCRUB_TILE,
  CLASS_ID_SEA_TILE,
  CLASS_ID_SHOALS_TILE,
  CLASS_ID_SITE_OF_DEATH_TILE,
  CLASS_ID_SPRINGS_TILE,
  CLASS_ID_STAIRCASE_TILE,
  CLASS_ID_TEMPLE_TILE,
  CLASS_ID_TREE_TILE,
  CLASS_ID_UP_STAIRCASE_TILE,
  CLASS_ID_VILLAGE_TILE,
  CLASS_ID_WEEDS_TILE,
  CLASS_ID_WHEAT_TILE,
  CLASS_ID_WORLD_MAP_TILE,
  CLASS_ID_WORLD_MAP_LANDMARK_TILE,
  CLASS_ID_WORSHIP_SITE_TILE,
  CLASS_ID_FEATURE,
  CLASS_ID_ALTAR,
  CLASS_ID_ENTRANCE,
  CLASS_ID_BENCH,
  CLASS_ID_DECORATIVE_STATUE,
  CLASS_ID_KING_DECORATIVE_STATUE,
  CLASS_ID_QUEEN_DECORATIVE_STATUE,
  CLASS_ID_WARLORD_DECORATIVE_STATUE,
  CLASS_ID_KNIGHT_DECORATIVE_STATUE,
  CLASS_ID_HIGH_PRIEST_DECORATIVE_STATUE,
  CLASS_ID_SORCEROR_DECORATIVE_STATUE,
  CLASS_ID_DOOR,
  CLASS_ID_PEW,
  CLASS_ID_EAST_WEST_PEW,
  CLASS_ID_ENTRANCE_STATE,
  CLASS_ID_GOOD_ALTAR,
  CLASS_ID_NEUTRAL_ALTAR,
  CLASS_ID_EVIL_ALTAR,
  CLASS_ID_FIRE_PILLAR,
  CLASS_ID_FOUNTAIN,
  CLASS_ID_GATE,
  CLASS_ID_LOCK,
  CLASS_ID_TRAP,
  CLASS_ID_PLAYER_DECISION_STRATEGY,
  CLASS_ID_IMMOBILE_DECISION_STRATEGY,
  CLASS_ID_MOBILE_DECISION_STRATEGY,
  CLASS_ID_CURSES_KEYBOARD_CONTROLLER,
  CLASS_ID_NULL_KEYBOARD_CONTROLLER,
  CLASS_ID_THREAT_RATINGS,
  CLASS_ID_CREATURE,
  CLASS_ID_STATISTIC,
  CLASS_ID_DICE,
  CLASS_ID_DAMAGE,
  CLASS_ID_MOVEMENT_ACCUMULATION,
  CLASS_ID_RELIGION,
  CLASS_ID_DEITY_STATUS,
  CLASS_ID_RESISTANCES,
  CLASS_ID_SLASH_RESISTANCE,
  CLASS_ID_PIERCE_RESISTANCE,
  CLASS_ID_POUND_RESISTANCE,
  CLASS_ID_HEAT_RESISTANCE,
  CLASS_ID_COLD_RESISTANCE,
  CLASS_ID_ACID_RESISTANCE,
  CLASS_ID_POISON_RESISTANCE,
  CLASS_ID_HOLY_RESISTANCE,
  CLASS_ID_SHADOW_RESISTANCE,
  CLASS_ID_ARCANE_RESISTANCE,
  CLASS_ID_MENTAL_RESISTANCE,
  CLASS_ID_SONIC_RESISTANCE,
  CLASS_ID_RADIANT_RESISTANCE,
  CLASS_ID_LIGHTNING_RESISTANCE,
  CLASS_ID_SKILLS,
  CLASS_ID_SKILL_ARCHERY,
  CLASS_ID_SKILL_AWARENESS,
  CLASS_ID_SKILL_BARGAINING,
  CLASS_ID_SKILL_BEASTMASTERY,
  CLASS_ID_SKILL_BLINDFIGHTING,
  CLASS_ID_SKILL_BOATING,
  CLASS_ID_SKILL_BOWYER,
  CLASS_ID_SKILL_BREWING,
  CLASS_ID_SKILL_CANTRIPS,
  CLASS_ID_SKILL_CARRYING,
  CLASS_ID_SKILL_COMBAT,
  CLASS_ID_SKILL_CRAFTING,
  CLASS_ID_SKILL_DESERT_LORE,
  CLASS_ID_SKILL_DETECTION,
  CLASS_ID_SKILL_DISARM_TRAPS,
  CLASS_ID_SKILL_DUAL_WIELD,
  CLASS_ID_SKILL_DUNGEONEERING,
  CLASS_ID_SKILL_ESCAPE,
  CLASS_ID_SKILL_FISHING,
  CLASS_ID_SKILL_FLETCHERY,
  CLASS_ID_SKILL_FORAGING,
  CLASS_ID_SKILL_FOREST_LORE,
  CLASS_ID_SKILL_HERBALISM,
  CLASS_ID_SKILL_HIDING,
  CLASS_ID_SKILL_HUNTING,
  CLASS_ID_SKILL_INTIMIDATION,
  CLASS_ID_SKILL_JEWELER,
  CLASS_ID_SKILL_JUMPING,
  CLASS_ID_SKILL_LEADERSHIP,
  CLASS_ID_SKILL_LITERACY,
  CLASS_ID_SKILL_LORE,
  CLASS_ID_SKILL_MAGICGENERAL,
  CLASS_ID_SKILL_MARSH_LORE,
  CLASS_ID_SKILL_MEDICINE,
  CLASS_ID_SKILL_MOUNTAIN_LORE,
  CLASS_ID_SKILL_MOUNTAINEERING,
  CLASS_ID_SKILL_MUSIC,
  CLASS_ID_SKILL_NIGHT_SIGHT,
  CLASS_ID_SKILL_OCEANOGRAPHY,
  CLASS_ID_SKILL_PAPERCRAFT,
  CLASS_ID_SKILL_RELIGION,
  CLASS_ID_SKILL_SCRIBING,
  CLASS_ID_SKILL_SKINNING,
  CLASS_ID_SKILL_SMITHING,
  CLASS_ID_SKILL_SPELUNKING,
  CLASS_ID_SKILL_STEALTH,
  CLASS_ID_SKILL_SWIMMING,
  CLASS_ID_SKILL_TANNING,
  CLASS_ID_SKILL_THIEVERY,
  CLASS_ID_SKILL_WEAVING,
  CLASS_ID_SKILL_AXES,
  CLASS_ID_SKILL_SHORT_BLADES,
  CLASS_ID_SKILL_LONG_BLADES,
  CLASS_ID_SKILL_BLUDGEONS,
  CLASS_ID_SKILL_DAGGERS,
  CLASS_ID_SKILL_RODS_AND_STAVES,
  CLASS_ID_SKILL_SPEARS,
  CLASS_ID_SKILL_UNARMED,
  CLASS_ID_SKILL_WHIPS,
  CLASS_ID_SKILL_EXOTIC,
  CLASS_ID_SKILL_THROWN_AXES,
  CLASS_ID_SKILL_THROWN_BLADES,
  CLASS_ID_SKILL_THROWN_BLUDGEONS,
  CLASS_ID_SKILL_BOWS,
  CLASS_ID_SKILL_CROSSBOWS,
  CLASS_ID_SKILL_THROWN_DAGGERS,
  CLASS_ID_SKILL_ROCKS,
  CLASS_ID_SKILL_SLINGS,
  CLASS_ID_SKILL_THROWN_SPEARS,
  CLASS_ID_SKILL_THROWN_EXOTIC,
  CLASS_ID_SKILL_ARCANE,
  CLASS_ID_SKILL_DIVINE,
  CLASS_ID_SKILL_MYSTIC,
  CLASS_ID_SKILL_PRIMORDIAL,
  CLASS_ID_WEIGHT,
  CLASS_ID_AMMUNITION,
  CLASS_ID_AMULET,
  CLASS_ID_ARMOUR,
  CLASS_ID_BOAT,
  CLASS_ID_BOOK,
  CLASS_ID_CURRENCY,
  CLASS_ID_TOOL,
  CLASS_ID_FOOD,
  CLASS_ID_MISC_ITEM,
  CLASS_ID_PLANT,
  CLASS_ID_POTION,
  CLASS_ID_RING,
  CLASS_ID_SCROLL,
  CLASS_ID_STAFF,
  CLASS_ID_WAND,
  CLASS_ID_MELEE_WEAPON,
  CLASS_ID_RANGED_WEAPON,
  CLASS_ID_INVENTORY,
  CLASS_ID_EQUIPMENT,
  CLASS_ID_METADATA,
  CLASS_ID_HUNGER_CLOCK,
  CLASS_ID_CREATURE_GENERATION_VALUES
};
