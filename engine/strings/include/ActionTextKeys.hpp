#pragma once
#include <string>
#include "Spell.hpp"
#include "WaterTypes.hpp"

class ActionTextKeys
{
  public:
    static std::string get_evoke_message(const std::string& desc_sid, const std::string& wand_desc, const bool is_player);
    static std::string get_quaff_message(const std::string& desc_sid, const std::string& potion_desc, const bool is_player);
    static std::string get_read_message(const std::string& player_read_sid, const std::string& monster_read_sid, const std::string& desc_sid, const std::string& readable_desc, const bool is_player);
    static std::string get_eat_message(const std::string& desc_sid, const std::string& consumable_desc, const bool is_player);
    static std::string get_eat_confirmation_message(const std::string& consumable_desc);
    static std::string get_full_message(const std::string& desc_sid, const std::string& consumable_desc, const bool is_player);
    static std::string get_spellcasting_message(const Spell& spell, const std::string& creature_desc_s, const bool is_player);
    static std::string get_spellcasting_cancelled_message(const std::string& creature_desc_sid, const bool is_player);
    static std::string get_bestiary_search_message(const std::string& prev_search_text);
    static std::string get_pour_message(const std::string& desc_sid, const std::string& item_desc_sid, const bool is_player);
    static std::string get_stumble_message(const std::string& desc_sid, const bool is_player);
    static std::string get_alcohol_poisoning_death_message(const std::string& desc_sid, const bool is_player);
    static std::string get_kick_message(const std::string& desc_sid, const bool is_player);
    static std::string get_kick_object_message(const std::string& desc_sid, const std::string& object_desc, const bool is_player);
    static std::string get_knock_back_message(const std::string& attacked_desc_sid, const bool is_player);
    static std::string get_generic_wear_off_message(const std::string& desc_sid); // monster only
    static std::string get_item_breakage_message(const std::string& creature_desc_sid, const bool is_player, const std::string& item_desc);
    static std::string get_spit_out_message(const std::string& seed_usage_desc);
    static std::string get_seed_planted_message(const bool blind, const std::string& seed_usage_desc);
    static std::string get_random_bait_message();
    static std::string get_fishing_outcome_message(const FishingOutcomeType fot);
    static std::string get_already_stolen_message(const std::string& desc);
    static std::string get_no_pockets_message(const std::string& desc);
    static std::string get_steal_successful_message(const std::string& creature_desc, const std::string& item_desc, const bool is_player);
    static std::string get_steal_unsuccessful_message(const std::string& stealing_desc, const std::string& steal_desc, const bool is_player);
    static std::string get_cry_out_message(const std::string& attacked_desc);

    static const std::string ACTION_NOT_FOUND;
    static const std::string ACTION_SEARCH;
    static const std::string ACTION_SEARCH_FOUND;
    static const std::string ACTION_SEARCH_FOUND_MULTIPLE;
    static const std::string ACTION_SEARCH_BLIND;
    static const std::string ACTION_PICK_UP_NOT_ALLOWED;
    static const std::string ACTION_DROP_NOT_ALLOWED;
    static const std::string ACTION_DROP_NO_ITEM_SELECTED;
    static const std::string ACTION_DROP_QUANTITY_PROMPT;
    static const std::string ACTION_DROP_INVALID_QUANTITY;
    static const std::string ACTION_PICK_UP_NOTHING_ON_GROUND;
    static const std::string ACTION_PICK_UP_MAX_ITEMS;
    static const std::string ACTION_PICK_UP_MAX_WEIGHT;
    static const std::string ACTION_MOVEMENT_BLOCKED;
    static const std::string ACTION_PLAYER_DROWNING;
    static const std::string ACTION_PLAYER_FALLING;
    static const std::string ACTION_PLAYER_OBLITERATED;
    static const std::string ACTION_PLAYER_FALL_FROM_MOUNTAIN;
    static const std::string ACTION_PLAYER_STARVING_TO_DEATH;
    static const std::string ACTION_DETECTED_OUT_OF_DEPTH_CREATURES;
    static const std::string ACTION_LOOK;
    static const std::string ACTION_LOOK_UNEXPLORED_TILE;
    static const std::string ACTION_LOOK_TILE_OUT_OF_RANGE;
    static const std::string ACTION_FIRE;
    static const std::string ACTION_NOTHING_HAPPENS;
    static const std::string ACTION_CHAT_NOBODY_NEARBY;
    static const std::string ACTION_CHAT_TIMEWALK;
    static const std::string ACTION_CHAT_NO_RESPONSE;
    static const std::string ACTION_GET_DIRECTION;
    static const std::string ACTION_GET_CARDINAL_DIRECTION;
    static const std::string ACTION_OPEN_DOOR;
    static const std::string ACTION_CLOSE_DOOR;
    static const std::string ACTION_OPEN_SARCOPHAGUS;
    static const std::string ACTION_OPEN_SARCOPHAGUS_CONFIRM;
    static const std::string ACTION_OPEN_SARCOPHAGUS_ALREADY_OPEN;
    static const std::string ACTION_DOOR_DESTROYED;
    static const std::string ACTION_APPLY_NO_FEATURES_PRESENT;
    static const std::string ACTION_HANDLE_LOCK;
    static const std::string ACTION_HANDLE_LOCK_NO_KEY;
    static const std::string ACTION_BESTIARY_DISPLAY_COMMAND_FOR_TILE_SELECTION;
    static const std::string ACTION_BESTIARY_NO_SUCH_CREATURE_EXISTS;
    static const std::string ACTION_POTION_EXPLODES;
    static const std::string ACTION_AUTOMOVE_WORLD_MAP;
    static const std::string ACTION_AUTOMOVE_TOO_HUNGRY;
    static const std::string ACTION_AUTOMOVE_BAD_STATUSES;
    static const std::string ACTION_AUTOMOVE_HOSTILE_CREATURES;
    static const std::string ACTION_KICK_WORLD_MAP;
    static const std::string ACTION_KICK_NOTHING_TO_KICK_AT;
    static const std::string ACTION_KICK_SOLID_TILE;
    static const std::string ACTION_KICK_WATER_TILE;
    static const std::string ACTION_KICK_FORGE;
    static const std::string ACTION_KICK_JEWELER_WORKBENCH;
    static const std::string ACTION_KICK_TANNERY;
    static const std::string ACTION_KICK_WHEEL_AND_LOOM;
    static const std::string ACTION_KICK_BARREL;
    static const std::string ACTION_KICK_SARCOPHAGUS;
    static const std::string ACTION_KICK_TRAP;
    static const std::string ACTION_KICK_PASSES_THROUGH;
    static const std::string ACTION_KICK_DOOR_DESTROYED;
    static const std::string ACTION_KICK_DOOR_BUCKLED;
    static const std::string ACTION_KICK_DOOR_UNMOVED;
    static const std::string ACTION_KICK_SPRAIN_LEG;
    static const std::string ACTION_APPLY_TRAP_TOO_FAR;
    static const std::string ACTION_FORGE_NO_SUITABLE_HAMMER;
    static const std::string ACTION_FORGE_NO_SMITHING_SKILL;
    static const std::string ACTION_FORGE_NO_INGOTS;
    static const std::string ACTION_FORGE_ARTIFACT_SELECTED;
    static const std::string ACTION_FORGE_FULL_POTENTIAL;
    static const std::string ACTION_FORGE_SUCCESSFUL;
    static const std::string ACTION_JEWELER_WORKBENCH_NO_JEWELER_SKILL;
    static const std::string ACTION_SKIN_WORLD_MAP;
    static const std::string ACTION_SKIN_NO_CORPSES;
    static const std::string ACTION_SKIN_MANGLED_CORPSE;
    static const std::string ACTION_SKIN_SUCCESSFUL;
    static const std::string ACTION_SKIN_NO_SKINS;
    static const std::string ACTION_SKIN_CREATE_ARMOUR;
    static const std::string ACTION_WEAVE_NO_FIBRES;
    static const std::string ACTION_WEAVE_NO_WEAVING_SKILL;
    static const std::string ACTION_WEAVE_SUCCESSFUL;
    static const std::string ACTION_SQUEEZE_FAILURE;
    static const std::string ACTION_DIG_WATER;
    static const std::string ACTION_DIG_AIR;
    static const std::string ACTION_DIG_GROUND;
    static const std::string ACTION_DIG_GRAVE;
    static const std::string ACTION_DIG_UNDEAD_FOUND;
    static const std::string ACTION_DIG_ITEM_FOUND;
    static const std::string ACTION_DIG_THROUGH_TILE;
    static const std::string ACTION_DIG_CANNOT_DIG;
    static const std::string ACTION_DIG_CANNOT_DIG_ON_SUPER_TYPE;
    static const std::string ACTION_DIG_TOO_HARD;
    static const std::string ACTION_RESTING;
    static const std::string ACTION_HANDLE_DEFAULT;
    static const std::string ACTION_HANDLE_ALTAR;
    static const std::string ACTION_HANDLE_PEW;
    static const std::string ACTION_EVOKE_FAILED;
    static const std::string ACTION_KEYBINDINGS;
    static const std::string ACTION_INTRODUCTION_ROGUELIKES;
    static const std::string ACTION_SOTW_HISTORY;
    static const std::string ACTION_STRATEGY_BASICS;
    static const std::string ACTION_FISHING_NO_WATER;
    static const std::string ACTION_FISHING_NO_EQUIPMENT;
    static const std::string ACTION_FISHING_BAIT1;
    static const std::string ACTION_FISHING_BAIT2;
    static const std::string ACTION_FISHING_BAIT3;
    static const std::string ACTION_FISHING_NO_CATCH;
    static const std::string ACTION_FISHING_ESCAPE;
    static const std::string ACTION_FISHING_NIBBLE;
    static const std::string ACTION_FISHING_CATCH;
    static const std::string ACTION_FISHING_THROW_BACK;
    static const std::string ACTION_DISARM_TRAPS_NO_TRAPS;
    static const std::string ACTION_DISARM_TRAPS_NO_TRAP;
    static const std::string ACTION_DISARM_TRAPS_OUTCOME_DISMANTLE;
    static const std::string ACTION_DISARM_TRAPS_OUTCOME_DISARM;
    static const std::string ACTION_DISARM_TRAPS_OUTCOME_FAIL;
    static const std::string ACTION_DISARM_TRAPS_OUTCOME_TRIGGER;
    static const std::string ACTION_THIEVERY_NO_TARGETS;
    static const std::string ACTION_THIEVERY_SELF_TARGET;
    static const std::string ACTION_INSCRIBE_WORLD_MAP;
    static const std::string ACTION_INSCRIBE_WATER;
    static const std::string ACTION_INSCRIBE_AIR;
    static const std::string ACTION_INSCRIBE_GROUND;
    static const std::string ACTION_INSCRIBE_QUERY;
    static const std::string ACTION_POUR_EMPTY;
    static const std::string ACTION_WANDCRAFT_INSUFFICIENT_COMPONENTS;
    static const std::string ACTION_WANDCRAFT_INSUFFICIENT_CASTINGS;
    static const std::string ACTION_WANDCRAFT_NO_SPELLS;
    static const std::string ACTION_WANDCRAFT_WAND_CREATED;
    static const std::string ACTION_SCRIBING_INSUFFICIENT_COMPONENTS;
    static const std::string ACTION_SCRIBING_INSUFFICIENT_CASTINGS;
    static const std::string ACTION_SCRIBING_NO_SPELLS;
    static const std::string ACTION_SCRIBING_SCROLL_CREATED;

  protected:
    ActionTextKeys();
    ~ActionTextKeys();

    static std::string get_general_message(const std::string& desc_sid, const std::string& player_message_sid, const std::string& monster_message_sid, const bool is_player);
    static std::string get_general_action_message(const std::string& desc_sid, const std::string& item_desc /* not a sid! */, const std::string& player_message_sid, const std::string& monster_message_sid, const bool is_player);
    
    static const std::string ACTION_EVOKE_PLAYER;
    static const std::string ACTION_EVOKE_MONSTER;
    static const std::string ACTION_QUAFF_PLAYER;
    static const std::string ACTION_QUAFF_MONSTER;
    static const std::string ACTION_EAT_PLAYER;
    static const std::string ACTION_EAT_MONSTER;
    static const std::string ACTION_EAT_CONFIRMATION;
    static const std::string ACTION_FULL_PLAYER;
    static const std::string ACTION_FULL_MONSTER;
    static const std::string ACTION_SPELLCASTING_CANCELLED_PLAYER;
    static const std::string ACTION_SPELLCASTING_CANCELLED_MONSTER;
    static const std::string ACTION_BESTIARY_WHICH_CREATURE;
    static const std::string ACTION_BESTIARY_PREVIOUS_SEARCH_MESSAGE;
    static const std::string ACTION_POUR_PLAYER;
    static const std::string ACTION_POUR_MONSTER;
    static const std::string ACTION_STUMBLE_PLAYER;
    static const std::string ACTION_STUMBLE_MONSTER;
    static const std::string ACTION_ALCOHOL_POISONING_PLAYER;
    static const std::string ACTION_ALCOHOL_POISONING_MONSTER;
    static const std::string ACTION_ENCHANT_ITEM_MAX;
    static const std::string ACTION_KICK_PLAYER;
    static const std::string ACTION_KICK_MONSTER;
    static const std::string ACTION_KICK_OBJECT_PLAYER;
    static const std::string ACTION_KICK_OBJECT_MONSTER;
    static const std::string ACTION_KNOCK_BACK_PLAYER;
    static const std::string ACTION_KNOCK_BACK_MONSTER;
    static const std::string ACTION_SPELL_WEAR_OFF_MONSTER;
    static const std::string ACTION_ITEM_BREAKAGE_PLAYER;
    static const std::string ACTION_ITEM_BREAKAGE_MONSTER;
    static const std::string ACTION_SPIT_OUT_SEED;
    static const std::string ACTION_SEED_PLANTED;
    static const std::string ACTION_THIEVERY_ALREADY_STOLEN;
    static const std::string ACTION_THIEVERY_NO_POCKETS;
    static const std::string ACTION_THIEVERY_STEAL_SUCCESSFUL_PLAYER;
    static const std::string ACTION_THIEVERY_STEAL_SUCCESSFUL_MONSTER;
    static const std::string ACTION_THIEVERY_STEAL_UNSUCCESSFUL_PLAYER;
    static const std::string ACTION_THIEVERY_STEAL_UNSUCCESSFUL_MONSTER;
    static const std::string ACTION_CRY_OUT;
};
