#include "LuaAPIFunctions.hpp"
#include "ClassManager.hpp"
#include "CreatureFactory.hpp"
#include "ExperienceManager.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Log.hpp"
#include "LuaUtils.hpp"
#include "ItemManager.hpp"
#include "MapExitUtils.hpp"
#include "MapUtils.hpp"
#include "MessageManagerFactory.hpp"
#include "PlayerConstants.hpp"
#include "Quests.hpp"
#include "RNG.hpp"
#include "StatisticTextKeys.hpp"
#include "StatusEffectFactory.hpp"
#include "StringTable.hpp"
#include "TextMessages.hpp"
#include "TileGenerator.hpp"

using namespace std;

CreaturePtr local_creature;

void set_local_creature(CreaturePtr creature)
{
  local_creature = creature;
}

// Get the local creature, if it matches, or from the current map
// otherwise.  This allows lookup of a local creature before the
// game has been set up (so that level-up scripts can be run on
// creatures on startup, etc).
CreaturePtr get_creature(const string& creature_id)
{
  // Check to see if there is a local creature defined, and if
  // it has the ID we're looking for.
  //
  // Otherwise, check the Game's current map, as usual.
  if (local_creature && (creature_id == local_creature->get_id()))
  {
    return local_creature;
  }
  else
  {
    CreatureMap& cmap = Game::instance().get_current_map()->get_creatures_ref();
    CreatureMap::iterator c_it = cmap.find(creature_id);

    if (c_it != cmap.end())
    {
      CreaturePtr creature = c_it->second;
      return creature;
    }
  }

  CreaturePtr null_creature;
  return null_creature;
}

// Get a particular tile from the given map
TilePtr get_tile(const string& map_id, const Coordinate& c)
{

  Game& game = Game::instance();
  MapRegistry& mr = game.get_map_registry_ref();
  MapPtr map = mr.get_map(map_id);

  TilePtr tile;

  if (map)
  {
    tile = map->at(c);
  }

  return tile;
}

// Register all the functions available to the scripting engine.
void ScriptEngine::register_api_functions()
{
  lua_register(L, "add_message_with_pause", add_message_with_pause);
  lua_register(L, "clear_and_add_message", clear_and_add_message);
  lua_register(L, "add_message", add_message);
  lua_register(L, "add_debug_message", add_debug_message);
  lua_register(L, "add_confirmation_message", add_confirmation_message);
  lua_register(L, "add_new_quest", add_new_quest);
  lua_register(L, "is_on_quest", is_on_quest);
  lua_register(L, "get_num_creature_killed_global", get_num_creature_killed_global);
  lua_register(L, "add_object_to_player_tile", add_object_to_player_tile);
  lua_register(L, "mark_quest_completed", mark_quest_completed);
  lua_register(L, "remove_active_quest", remove_active_quest);
  lua_register(L, "is_quest_completed", is_quest_completed);
  lua_register(L, "player_has_item", player_has_item);
  lua_register(L, "remove_object_from_player", remove_object_from_player);
  lua_register(L, "is_item_generated", is_item_generated);
  lua_register(L, "get_num_item_generated", get_num_item_generated);
  lua_register(L, "set_skill_value", set_skill_value);
  lua_register(L, "get_skill_value", get_skill_value);
  lua_register(L, "RNG_range", RNG_range);
  lua_register(L, "RNG_percent_chance", RNG_percent_chance);
  lua_register(L, "add_spell_castings", add_spell_castings);
  lua_register(L, "gain_experience", gain_experience);
  lua_register(L, "add_creature_to_map", add_creature_to_map);
  lua_register(L, "add_status_to_creature", add_status_to_creature);
  lua_register(L, "stop_playing_game", stop_playing_game);
  lua_register(L, "set_creature_base_damage", set_creature_base_damage);
  lua_register(L, "set_creature_speed", set_creature_speed);
  lua_register(L, "get_creature_speed", get_creature_speed);
  lua_register(L, "get_creature_yx", get_creature_yx);
  lua_register(L, "get_current_map_id", get_current_map_id);
  lua_register(L, "gain_level", gain_level);
  lua_register(L, "goto_level", goto_level);
  lua_register(L, "is_player", is_player);
  lua_register(L, "incr_str", incr_str);
  lua_register(L, "incr_dex", incr_dex);
  lua_register(L, "incr_agi", incr_agi);
  lua_register(L, "incr_hea", incr_hea);
  lua_register(L, "incr_int", incr_int);
  lua_register(L, "incr_wil", incr_wil);
  lua_register(L, "incr_cha", incr_cha);
  lua_register(L, "map_set_custom_map_id", map_set_custom_map_id);
  lua_register(L, "map_set_edesc", map_set_edesc);
  lua_register(L, "map_set_additional_property", map_set_additional_property);
  lua_register(L, "map_add_location", map_add_location);
  lua_register(L, "map_transform_tile", map_transform_tile);
  lua_register(L, "map_add_tile_exit", map_add_tile_exit);
  lua_register(L, "log", log);
  lua_register(L, "get_player_title", get_player_title);
}

// Lua API helper functions
string read_sid_and_replace_values(lua_State* ls)
{
  string message;

  int num_args = lua_gettop(ls);
  if (num_args > 0 && lua_isstring(ls, 1))
  {
    vector<string> replacement_sids;
    string message_sid = lua_tostring(ls, 1);

    if (num_args == 2)
    {
      replacement_sids = LuaUtils::get_string_array_from_table(ls, 2);
    }

    message = StringTable::get(message_sid);

    for (const auto& value : replacement_sids)
    {
      boost::replace_first(message, "%s", value);
    }
  }

  return message;
}

// Lua API functions:

// Functions callable by Lua that wrap the actual C++ functions for adding
// messages, getting/updating data, etc.

// Clear the message manager, add a new message, and force the user to 
// continue via a "..." type prompt.
// 
// Arguments are:
// - 1: string containing the SID
// - 2: string array containing replacement values (optional)
static int add_message_with_pause(lua_State* ls)
{
  int num_args = lua_gettop(ls);
  if (num_args > 0 && lua_isstring(ls, 1))
  {
    string message = read_sid_and_replace_values(ls);

    IMessageManager& manager = MessageManagerFactory::instance();
    manager.clear_if_necessary();
    manager.add_new_message_with_pause(message);
    manager.send();

    // Because this function can only be called in a quest context,
    // where the speaking player is always assumed to be the player,
    // directly getting the player and getting its decision is safe
    // for now.
    Game& game = Game::instance();
    game.get_current_player()->get_decision_strategy()->get_confirmation();
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_message_with_pause");
    lua_error(ls);
  }

  return 0;
}

// Clear the message manager and add a new message.
// Arguments expected: 1-2.
// Argument types: 
// - 1: string (resource SID)
// - 2: string array, containing replacement values (optional)
static int clear_and_add_message(lua_State* ls)
{
  int num_args = lua_gettop(ls);
  if (num_args > 0 && lua_isstring(ls, 1))
  {
    string message = read_sid_and_replace_values(ls);

    IMessageManager& manager = MessageManagerFactory::instance();
    manager.clear_if_necessary();
    manager.add_new_message(message);
    manager.send();
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to clear_and_add_message");
    lua_error(ls);
  }

  return 0;
}

// Add a new message.
// Arguments expected: 1-2.
// Argument types: string (resource SID, required), table of strings (opt.)
// Assumption: table of strings is an array.
static int add_message(lua_State* ls)
{
  int num_args = lua_gettop(ls);
  if (num_args > 0 && lua_isstring(ls, 1))
  {
    string message = read_sid_and_replace_values(ls);

    IMessageManager& manager = MessageManagerFactory::instance();
    manager.add_new_message(message);
    manager.send();
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_message");
    lua_error(ls);
  }

  return 0;
}

// Clear the message manager, and add a new message.
// Arguments expected: 1-2
// Argument type: 
//    1: string (not a resource string)
//    2: array of strings for replacement (optional)
// The expectation is that this function should be used only for
// debugging purposes, where strings won't be in the .ini files - use the regular 
// "add_message" function otherwise!
static int add_debug_message(lua_State* ls)
{
  int num_args = lua_gettop(ls);

  if (num_args > 0 && lua_isstring(ls, 1))
  {
    vector<string> replacement_sids;
    string debug = lua_tostring(ls, 1);

    if (num_args == 2)
    {
      replacement_sids = LuaUtils::get_string_array_from_table(ls, 2);
    }

    for (const auto& value : replacement_sids)
    {
      boost::replace_first(debug, "%s", value);
    }

    IMessageManager& manager = MessageManagerFactory::instance();
    manager.clear_if_necessary();
    manager.add_new_message(debug);
    manager.send();
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_debug_message");
    lua_error(ls);
  }

  return 0;
}

// Add a message with a confirmation prompt at the end.
// Arguments: message SID.
// Return value: boolean
static int add_confirmation_message(lua_State* ls)
{
  bool confirm = false;

  if ((lua_gettop(ls) > 0) && (lua_isstring(ls, 1)))
  {
    Game& game = Game::instance();
    CreaturePtr player = game.get_current_player();
    string message_sid = lua_tostring(ls, 1);

    IMessageManager& manager = MessageManagerFactory::instance();
    manager.clear_if_necessary();
    manager.add_new_confirmation_message(TextMessages::get_confirmation_message(message_sid));
    confirm = player->get_decision_strategy()->get_confirmation();

    manager.send();
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_confirmation_message");
    lua_error(ls);
  }

  lua_pushboolean(ls, confirm);
  return 1;
}

// Add a quest to the in-progress list.  Arguments are:
// - a string representing the quest ID
// - a table representing the "Quest" lua object.
static int add_new_quest(lua_State* ls)
{
  if ((lua_gettop(ls) == 2) && (lua_istable(ls, -1)) && (lua_isstring(ls, -2)))
  {
    Game& game = Game::instance();
    ScriptEngine& se = game.get_script_engine_ref();

    string quest_id = lua_tostring(ls, 1);
    string quest_title_sid = se.get_table_str(ls, "quest_title_sid");
    string questmaster_name_sid = se.get_table_str(ls, "questmaster_name_sid");
    string quest_description_sid = se.get_table_str(ls, "quest_description_sid");

    Quest new_quest(quest_id, quest_title_sid, questmaster_name_sid, quest_description_sid);

    game.get_quests_ref().add_new_quest(quest_id, new_quest);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_new_quest");
    lua_error(ls);
  }

  // Return nothing.
  return 0;
}

// Check to see if a current quest is in progress.
// Argument is a string representing the quest ID.
int is_on_quest(lua_State* ls)
{
  bool quest_in_progress = false;

  if ((lua_gettop(ls) == 1) && (lua_isstring(ls, -1)))
  {
    string quest_id = lua_tostring(ls, 1);

    // Check to see if the given quest ID is in progress.
    Game& game = Game::instance();
    Quests& quests = game.get_quests_ref();
    quest_in_progress = quests.is_quest_in_progress(quest_id);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to is_on_quest");
    lua_error(ls);
  }

  lua_pushboolean(ls, quest_in_progress);
  return 1;
}

// Check the global mortuary on the game object to determine the death count
// for a particular creature.
//
// Argument is the creature ID.
int get_num_creature_killed_global(lua_State* ls)
{
  int num_killed = 0;

  if ((lua_gettop(ls) == 1) && (lua_isstring(ls, -1)))
  {
    string creature_id = lua_tostring(ls, 1);

    Game& game = Game::instance();
    Mortuary& mort = game.get_mortuary_ref();
    num_killed = mort.get_num_creature_killed(creature_id);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to get_num_creature_killed_global");
    lua_error(ls);
  }

  lua_pushinteger(ls, num_killed);
  return 1;
}

// Add an object to the player's tile.
// Arguments:
// - 1: base item ID
// - 2: quantity (optional, 1 is assumed)
int add_object_to_player_tile(lua_State* ls)
{
  int num_args = lua_gettop(ls);

  if (lua_isstring(ls, 1) && (num_args == 1 || (num_args == 2 && lua_isnumber(ls, 2))))
  {
    string base_item_id;
    uint quantity = 1;

    Game& game = Game::instance();
    MapPtr map = game.get_current_map();
    CreaturePtr player = game.get_current_player();
    TilePtr player_tile = MapUtils::get_tile_for_creature(map, player);
    
    base_item_id = lua_tostring(ls, 1);

    // Set the quantity if it was specified.    
    if (num_args == 2) 
    {
      quantity = static_cast<uint>(lua_tointeger(ls, 2));
    }

    ItemManager::create_item_with_probability(100, 100, player_tile->get_items(), base_item_id, quantity);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_object_to_player_tile");
    lua_error(ls);
  }

  return 0;
}

// Mark a quest as completed.
// Argument is the quest ID.
int mark_quest_completed(lua_State* ls)
{
  if ((lua_gettop(ls) == 1) && (lua_isstring(ls, -1)))
  {
    string quest_id = lua_tostring(ls, 1);
    Game& game = Game::instance();
    Quests& quests = game.get_quests_ref();

    quests.set_quest_completed(quest_id);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to mark_quest_completed");
    lua_error(ls);
  }

  return 0;
}

// Remove an active quest (typically used for mutually exclusive quests)
// Argument is the quest ID.
int remove_active_quest(lua_State* ls)
{
  bool args_ok = ((lua_gettop(ls) == 1) && (lua_isstring(ls, -1)));
  bool quest_removed = false;

  if (args_ok)
  {
    string quest_id = lua_tostring(ls, 1);
    Game& game = Game::instance();
    Quests& quests = game.get_quests_ref();

    quests.remove_active_quest(quest_id);
  }

  if (!args_ok)
  {
    lua_pushstring(ls, "Incorrect arguments to remove_active_quest");
    lua_error(ls);
  }

  return 0;
}

// Check to see if a quest has been completed.
// Argument is the quest ID
int is_quest_completed(lua_State* ls)
{
  bool quest_completed = false;

  if ((lua_gettop(ls) == 1) && (lua_isstring(ls, -1)))
  {
    string quest_id = lua_tostring(ls, 1);
    Game& game = Game::instance();
    Quests& quests = game.get_quests_ref();

    quest_completed = quests.is_quest_completed(quest_id);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to is_quest_completed");
    lua_error(ls);
  }

  lua_pushboolean(ls, quest_completed);
  return 1;
}

// Check to see if the player has an item.
// Argument is the object's base ID.
int player_has_item(lua_State* ls)
{
  bool has_item = false;

  if ((lua_gettop(ls) == 1) && (lua_isstring(ls, -1)))
  {
    string base_item_id = lua_tostring(ls, 1);
    Game& game = Game::instance();
    CreaturePtr player = game.get_current_player();

    has_item = ItemManager::has_item(player, base_item_id);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to player_has_item");
    lua_error(ls);
  }

  lua_pushboolean(ls, has_item);
  return 1;
}

// Remove an object from the player's equipment or inventory
// Argument is the object's base ID.
int remove_object_from_player(lua_State* ls)
{
  if ((lua_gettop(ls) == 1) && (lua_isstring(ls, -1)))
  {
    string object_base_id = lua_tostring(ls, 1);

    Game& game = Game::instance();
    CreaturePtr player = game.get_current_player();

    ItemManager im;
    im.remove_item_from_eq_or_inv(player, object_base_id);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to remove_object_from_player");
    lua_error(ls);
  }

  return 0;
}

// Check to see if an item is generated.
// Argument is the base item ID.
int is_item_generated(lua_State* ls)
{
  bool item_generated = false;

  get_num_item_generated(ls);
  item_generated = (lua_tointeger(ls, -1) > 0);

  // We don't need this value anymore - calling function should get the 
  // boolean return value.
  lua_pop(ls, -1);

  lua_pushboolean(ls, item_generated);
  return 1;
}

// Check to see how many of a particular item have been generated.
// Argument is the base item ID.
int get_num_item_generated(lua_State* ls)
{
  int num_gen = 0;

  if ((lua_gettop(ls) == 1) && (lua_isstring(ls, -1)))
  {
    string object_base_id = lua_tostring(ls, 1);

    Game& game = Game::instance();
    GenerationValuesMap& item_generation_values = game.get_item_generation_values_ref();
    GenerationValuesMap::iterator i_it = item_generation_values.find(object_base_id);

    if (i_it != item_generation_values.end())
    {
      num_gen = i_it->second.get_current();
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to get_num_item_generated");
    lua_error(ls);
  }

  lua_pushnumber(ls, num_gen);
  return 1;
}

// Set a skill value for a particular creature.
// Three arguments are expected:
// - creature_id
// - skill enumeration value
// - new value (int) for that skill
int set_skill_value(lua_State* ls)
{
  if ((lua_gettop(ls) == 3) && (lua_isstring(ls, 1) && lua_isnumber(ls, 2) && lua_isnumber(ls, 3)))
  {
    string creature_id = lua_tostring(ls, 1);
    int skill_i = lua_tointeger(ls, 2);
    SkillType skill_name = static_cast<SkillType>(skill_i);
    int skill_value = lua_tointeger(ls, 3);

    CreaturePtr creature = get_creature(creature_id);

    if (creature)
    {
      creature->get_skills().set_value(skill_name, skill_value);
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to set_skill_value");
    lua_error(ls);
  }


  return 0;
}

// Get a skill value for a particular creature.
// Three arguments are expected:
// - creature_id of a creature on the current map
// - skill enumeration value
// Return value is hte current value (int) for that skill.
int get_skill_value(lua_State* ls)
{
  int skill_value = 0;

  if ((lua_gettop(ls) == 2) && (lua_isstring(ls, 1) && lua_isnumber(ls, 2)))
  {
    string creature_id = lua_tostring(ls, 1);
    int skill_i = lua_tointeger(ls, 2);
    SkillType skill_name = static_cast<SkillType>(skill_i);

    CreaturePtr creature = get_creature(creature_id);

    if (creature)
    {
      skill_value = creature->get_skills().get_value(skill_name);
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to get_skill_value");
    lua_error(ls);
  }

  lua_pushnumber(ls, skill_value);
  return 1;
}

int RNG_range(lua_State* ls)
{
  int rng_val = 0;

  if ((lua_gettop(ls) == 2) && (lua_isnumber(ls, 1) && lua_isnumber(ls, 2)))
  {
    int min = lua_tointeger(ls, 1);
    int max = lua_tointeger(ls, 2);

    rng_val = RNG::range(min, max);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to RNG_range");
    lua_error(ls);
  }

  lua_pushnumber(ls, rng_val);
  return 1;
}

int RNG_percent_chance(lua_State* ls)
{
  int rng_val = 0;

  if ((lua_gettop(ls) == 1) && (lua_isnumber(ls, 1)))
  {
    int percent = lua_tointeger(ls, 1);

    rng_val = RNG::percent_chance(percent);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to RNG_range");
    lua_error(ls);
  }

  lua_pushboolean(ls, rng_val);
  return 1;
}

// Add a given number of spell castings to the spell knowledge of a
// particular creature.
int add_spell_castings(lua_State* ls)
{
  if ((lua_gettop(ls) == 3) && (lua_isstring(ls, 1) && lua_isstring(ls, 2) && lua_isnumber(ls, 3)))
  {
    string creature_id = lua_tostring(ls, 1);
    string spell_id = lua_tostring(ls, 2);
    int addl_castings = lua_tointeger(ls, 3);

    CreaturePtr creature = get_creature(creature_id);
    SpellKnowledge& sk = creature->get_spell_knowledge_ref();

    IndividualSpellKnowledge isk = sk.get_spell_knowledge(spell_id);
    uint new_castings = isk.get_castings() + addl_castings;
    isk.set_castings(new_castings);
    sk.set_spell_knowledge(spell_id, isk);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_spell_castings");
    lua_error(ls);
  }

  return 0;
}

// Add a certain number of experience points to a particular creature.
int gain_experience(lua_State* ls)
{
  if ((lua_gettop(ls) == 2) && (lua_isstring(ls, 1) && lua_isnumber(ls, 2)))
  {
    string creature_id = lua_tostring(ls, 1);
    CreaturePtr creature = get_creature(creature_id);
    int experience = lua_tointeger(ls, 2);

    ExperienceManager em;
    em.gain_experience(creature, experience);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to gain_experience");
    lua_error(ls);
  }

  return 0;
}

// Add a creature to the map at a particular (y, x) coordinate
int add_creature_to_map(lua_State* ls)
{
  if ((lua_gettop(ls) == 3) && (lua_isstring(ls, 1) && lua_isnumber(ls, 2) && lua_isnumber(ls, 3)))
  {
    Game& game = Game::instance();
    MapPtr map = game.get_current_map();

    string creature_id = lua_tostring(ls, 1);

    CreatureFactory cf;
    CreaturePtr creature = cf.create_by_creature_id(game.get_action_manager_ref(), creature_id);
    Coordinate coords(lua_tointeger(ls, 2), lua_tointeger(ls, 3));

    if (creature && MapUtils::are_coordinates_within_dimensions(coords, map->size()))
    {
      GameUtils::add_new_creature_to_map(game, creature, map, coords);
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_creature_to_map");
    lua_error(ls);
  }

  return 0;
}

// Add a particular status to a particular creature for a particular duration
// in minutes.
//
// Returns true if the status was added, false in all other cases.
int add_status_to_creature(lua_State* ls)
{
  if ((lua_gettop(ls) == 2) && (lua_isstring(ls, 1) && (lua_isstring(ls, 2))))
  {
    Game& game = Game::instance();

    string creature_id = lua_tostring(ls, 1);
    string status_id = lua_tostring(ls, 2);
    CreaturePtr creature = get_creature(creature_id);

    if (creature && !creature->has_status(status_id))
    {
      StatusEffectPtr se = StatusEffectFactory::create_status_effect(status_id);
      se->apply_change(creature);

      lua_pushboolean(ls, true);
      return 1;
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to add_status_to_creature");
    lua_error(ls);
  }

  lua_pushboolean(ls, false);
  return 1;
}

// Set a creature's base (bare-handed) damage.
int set_creature_base_damage(lua_State* ls)
{
  if ((lua_gettop(ls) == 3) && (lua_isstring(ls, 1) && lua_isnumber(ls, 2) && lua_isnumber(ls, 3)))
  {
    string creature_id = lua_tostring(ls, 1);
    int num_dice = lua_tointeger(ls, 2);
    int num_sides = lua_tointeger(ls, 3);

    CreaturePtr creature = get_creature(creature_id);

    Damage damage = creature->get_base_damage();
    damage.set_num_dice(num_dice);
    damage.set_dice_sides(num_sides);

    creature->set_base_damage(damage);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to set_creature_base_damage");
    lua_error(ls);
  }

  return 0;
}

// Set a creature's speed.
int set_creature_speed(lua_State* ls)
{
  if ((lua_gettop(ls) == 2) && (lua_isstring(ls, 1) && lua_isnumber(ls, 2)))
  {
    string creature_id = lua_tostring(ls, 1);
    int new_base_speed = lua_tointeger(ls, 2);
    CreaturePtr creature = get_creature(creature_id);
    Statistic cr_speed = creature->get_speed();

    int cur_speed = cr_speed.get_current();

    cr_speed.set_base(new_base_speed);
    cr_speed.set_current(cur_speed - (cur_speed - new_base_speed));

    creature->set_speed(cr_speed);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to set_creature_speed");
    lua_error(ls);
  }

  return 0;
}

// Get a creature's speed.
int get_creature_speed(lua_State* ls)
{
  int speed = 0;

  if (lua_gettop(ls) == 1 && lua_isstring(ls, 1))
  {
    string creature_id = lua_tostring(ls, 1);
    CreaturePtr creature = get_creature(creature_id);
    speed = creature->get_speed().get_base();
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to get_creature_speed");
    lua_error(ls);
  }

  lua_pushnumber(ls, speed);
  return 1;
}

// Return the y and x coordinates for the given creature on the current map.
int get_creature_yx(lua_State* ls)
{
  int y = -1;
  int x = -1;

  if (lua_gettop(ls) == 1 && lua_isstring(ls, 1))
  {
    // Find the creature in the map
    string creature_id = lua_tostring(ls, 1);
    Coordinate c = Game::instance().get_current_map()->get_location(creature_id);

    // Set the return coordinates to the values from the lookup.
    y = c.first;
    x = c.second;
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to get_creature_yx");
    lua_error(ls);
  }

  lua_pushnumber(ls, y);
  lua_pushnumber(ls, x);
  return 2;
}

// Return the ID of the current map
int get_current_map_id(lua_State* ls)
{
  string map_id;

  if (lua_gettop(ls) == 0)
  {
    map_id = Game::instance().get_current_map()->get_map_id();
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to get_current_map_id");
    lua_error(ls);
  }

  lua_pushstring(ls, map_id.c_str());
  return 1;
}

int gain_level(lua_State* ls)
{
  if ((lua_gettop(ls) == 1) && lua_isstring(ls, 1))
  {
    string creature_id = lua_tostring(ls, 1);
    CreaturePtr creature = get_creature(creature_id);
    ExperienceManager em;

    if (creature)
    {
      em.gain_experience(creature, em.get_current_experience_needed_for_level(creature, creature->get_level().get_current() + 1));
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to gain_level");
    lua_error(ls);
  }

  return 0;
}

int goto_level(lua_State* ls)
{
  if (lua_gettop(ls) == 2 && lua_isstring(ls, 1) && lua_isnumber(ls, 2))
  {
    string creature_id = lua_tostring(ls, 1);
    uint glevel = lua_tointeger(ls, 2);
    CreaturePtr creature = get_creature(creature_id);
    uint level = creature->get_level().get_current();

    ExperienceManager em;

    if (creature)
    {
      for (uint cur_level = level; cur_level < glevel && cur_level < CreatureConstants::MAX_CREATURE_LEVEL; cur_level++)
      {
        em.gain_experience(creature, em.get_current_experience_needed_for_level(creature, cur_level + 1));
      }
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to goto_level");
    lua_error(ls);
  }

  return 0;
}

int is_player(lua_State* ls)
{
  bool is_creature_player = false;

  if (lua_gettop(ls) == 1 && lua_isstring(ls, 1))
  {
    string creature_id = lua_tostring(ls, 1);
    is_creature_player = (creature_id == PlayerConstants::PLAYER_CREATURE_ID);
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments supplied to is_player");
    lua_error(ls);
  }

  lua_pushboolean(ls, is_creature_player);
  return 1;
}

int incr_str(lua_State* ls)
{
  if (lua_gettop(ls) == 2 && lua_isstring(ls, 1) && lua_isboolean(ls, 2))
  {
    string creature_id = lua_tostring(ls, 1);
    int add_msg = lua_toboolean(ls, 2);

    CreaturePtr creature = get_creature(creature_id);
    Statistic cr_str = creature->get_strength();
    int new_base_str = cr_str.get_base() + 1;

    int cur_str = cr_str.get_current();

    cr_str.set_base(new_base_str);
    cr_str.set_current(cur_str - (cur_str - new_base_str));

    creature->set_strength(cr_str);

    if (add_msg)
    {
      IMessageManager& manager = MessageManagerFactory::instance();
      manager.add_new_message(StringTable::get(StatisticTextKeys::STATISTIC_GAIN_STRENGTH));
      manager.send();
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to incr_str");
    lua_error(ls);
  }

  return 0;
}

int incr_dex(lua_State* ls)
{
  if (lua_gettop(ls) == 2 && lua_isstring(ls, 1) && lua_isboolean(ls, 2))
  {
    string creature_id = lua_tostring(ls, 1);
    int add_msg = lua_toboolean(ls, 2);

    CreaturePtr creature = get_creature(creature_id);
    Statistic cr_dex = creature->get_dexterity();
    int new_base_dex = cr_dex.get_base() + 1;

    int cur_dex = cr_dex.get_current();

    cr_dex.set_base(new_base_dex);
    cr_dex.set_current(cur_dex - (cur_dex - new_base_dex));

    creature->set_dexterity(cr_dex);

    if (add_msg)
    {
      IMessageManager& manager = MessageManagerFactory::instance();
      manager.add_new_message(StringTable::get(StatisticTextKeys::STATISTIC_GAIN_DEXTERITY));
      manager.send();
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to incr_dex");
    lua_error(ls);
  }

  return 0;
}

int incr_agi(lua_State* ls)
{
  if (lua_gettop(ls) == 2 && lua_isstring(ls, 1) && lua_isboolean(ls, 2))
  {
    string creature_id = lua_tostring(ls, 1);
    int add_msg = lua_toboolean(ls, 2);

    CreaturePtr creature = get_creature(creature_id);
    Statistic cr_agi = creature->get_agility();
    int new_base_agi = cr_agi.get_base() + 1;

    int cur_agi = cr_agi.get_current();

    cr_agi.set_base(new_base_agi);
    cr_agi.set_current(cur_agi - (cur_agi - new_base_agi));

    creature->set_agility(cr_agi);

    if (add_msg)
    {
      IMessageManager& manager = MessageManagerFactory::instance();
      manager.add_new_message(StringTable::get(StatisticTextKeys::STATISTIC_GAIN_AGILITY));
      manager.send();
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to incr_agi");
    lua_error(ls);
  }

  return 0;
}

int incr_hea(lua_State* ls)
{
  if (lua_gettop(ls) == 2 && lua_isstring(ls, 1) && lua_isboolean(ls, 2))
  {
    string creature_id = lua_tostring(ls, 1);
    int add_msg = lua_toboolean(ls, 2);

    CreaturePtr creature = get_creature(creature_id);
    Statistic cr_hea = creature->get_health();
    int new_base_hea = cr_hea.get_base() + 1;

    int cur_hea = cr_hea.get_current();

    cr_hea.set_base(new_base_hea);
    cr_hea.set_current(cur_hea - (cur_hea - new_base_hea));

    creature->set_health(cr_hea);

    if (add_msg)
    {
      IMessageManager& manager = MessageManagerFactory::instance();
      manager.add_new_message(StringTable::get(StatisticTextKeys::STATISTIC_GAIN_HEALTH));
      manager.send();
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to incr_hea");
    lua_error(ls);
  }

  return 0;
}

int incr_int(lua_State* ls)
{
  if (lua_gettop(ls) == 2 && lua_isstring(ls, 1) && lua_isboolean(ls, 2))
  {
    string creature_id = lua_tostring(ls, 1);
    int add_msg = lua_toboolean(ls, 2);

    CreaturePtr creature = get_creature(creature_id);
    Statistic cr_int = creature->get_intelligence();
    int new_base_int = cr_int.get_base() + 1;

    int cur_int = cr_int.get_current();

    cr_int.set_base(new_base_int);
    cr_int.set_current(cur_int - (cur_int - new_base_int));

    creature->set_intelligence(cr_int);

    if (add_msg)
    {
      IMessageManager& manager = MessageManagerFactory::instance();
      manager.add_new_message(StringTable::get(StatisticTextKeys::STATISTIC_GAIN_INTELLIGENCE));
      manager.send();
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to incr_int");
    lua_error(ls);
  }

  return 0;
}

int incr_wil(lua_State* ls)
{
  if (lua_gettop(ls) == 2 && lua_isstring(ls, 1) && lua_isboolean(ls, 2))
  {
    string creature_id = lua_tostring(ls, 1);
    int add_msg = lua_toboolean(ls, 2);

    CreaturePtr creature = get_creature(creature_id);
    Statistic cr_wil = creature->get_willpower();
    int new_base_wil = cr_wil.get_base() + 1;

    int cur_wil = cr_wil.get_current();

    cr_wil.set_base(new_base_wil);
    cr_wil.set_current(cur_wil - (cur_wil - new_base_wil));

    creature->set_willpower(cr_wil);

    if (add_msg)
    {
      IMessageManager& manager = MessageManagerFactory::instance();
      manager.add_new_message(StringTable::get(StatisticTextKeys::STATISTIC_GAIN_WILLPOWER));
      manager.send();
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to incr_wil");
    lua_error(ls);
  }

  return 0;
}

int incr_cha(lua_State* ls)
{
  if (lua_gettop(ls) == 2 && lua_isstring(ls, 1) && lua_isboolean(ls, 2))
  {
    string creature_id = lua_tostring(ls, 1);
    int add_msg = lua_toboolean(ls, 2);

    CreaturePtr creature = get_creature(creature_id);
    Statistic cr_cha = creature->get_charisma();
    int new_base_cha = cr_cha.get_base() + 1;

    int cur_cha = cr_cha.get_current();

    cr_cha.set_base(new_base_cha);
    cr_cha.set_current(cur_cha - (cur_cha - new_base_cha));

    creature->set_charisma(cr_cha);

    if (add_msg)
    {
      IMessageManager& manager = MessageManagerFactory::instance();
      manager.add_new_message(StringTable::get(StatisticTextKeys::STATISTIC_GAIN_CHARISMA));
      manager.send();
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to incr_cha");
    lua_error(ls);
  }

  return 0;
}

// Set a custom map id into the given map id at the given row and col.
int map_set_custom_map_id(lua_State* ls)
{
  if (lua_gettop(ls) == 4 && lua_isstring(ls, 1) && lua_isnumber(ls, 2) && lua_isnumber(ls, 3) && lua_isstring(ls, 4))
  {
    string map_id = lua_tostring(ls, 1);
    Coordinate c(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
    string custom_map_id = lua_tostring(ls, 4);
    TilePtr tile = get_tile(map_id, c);

    if (tile)
    {
      tile->set_custom_map_id(custom_map_id);
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to map_set_custom_map_id");
    lua_error(ls);
  }

  return 0;
}

// Set an extra description SID into the given map id at the given row and col.
int map_set_edesc(lua_State* ls)
{
  if (lua_gettop(ls) == 4 && lua_isstring(ls, 1) && lua_isnumber(ls, 2) && lua_isnumber(ls, 3) && lua_isstring(ls, 4))
  {
    string map_id = lua_tostring(ls, 1);
    Coordinate c(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
    string extra_desc_sid = lua_tostring(ls, 4);
    TilePtr tile = get_tile(map_id, c);

    if (tile)
    {
      tile->set_extra_description_sid(extra_desc_sid);
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to map_set_edesc");
    lua_error(ls);
  }

  return 0;
}

// Set an additional property (k,v pair) into the given map id at the given
// row and column.
int map_set_additional_property(lua_State* ls)
{
  if (lua_gettop(ls) == 5 && lua_isstring(ls, 1) && lua_isnumber(ls, 2) && lua_isnumber(ls, 3) && lua_isstring(ls, 4) && lua_isstring(ls, 5))
  {
    string map_id = lua_tostring(ls, 1);
    Coordinate c(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
    string k = lua_tostring(ls, 4);
    string v = lua_tostring(ls, 5);
    TilePtr tile = get_tile(map_id, c);

    if (tile)
    {
      tile->set_additional_property(k, v);
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to map_set_additional_property");
    lua_error(ls);
  }

  return 0;
}

// Add a location to the given map
int map_add_location(lua_State* ls)
{
  if (lua_gettop(ls) == 4 && lua_isstring(ls, 1) && lua_isstring(ls, 2) && lua_isnumber(ls, 3) && lua_isnumber(ls, 4))
  {
    string map_id = lua_tostring(ls, 1);
    string loc = lua_tostring(ls, 2);
    Coordinate c(lua_tointeger(ls, 3), lua_tointeger(ls, 4));

    MapPtr map = Game::instance().get_map_registry_ref().get_map(map_id);

    if (map)
    {
      map->add_or_update_location(loc, c);
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to map_add_location");
    lua_error(ls);
  }

  return 0;
}

// Returns true if the tile was transformed, false otherwise.
int map_transform_tile(lua_State* ls)
{
  int result = false;

  if (lua_gettop(ls) == 4 && lua_isstring(ls, 1) && lua_isnumber(ls, 2) && lua_isnumber(ls, 3) && lua_isnumber(ls, 4))
  {
    string map_id = lua_tostring(ls, 1);
    Coordinate c(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
    int lua_tile_type = lua_tointeger(ls, 4);

    if (lua_tile_type >= TILE_TYPE_FIRST && lua_tile_type < TILE_TYPE_LAST)
    {
      MapPtr map = Game::instance().get_map_registry_ref().get_map(map_id);

      if (map != nullptr)
      {
        TilePtr tile = map->at(c);

        if (tile != nullptr)
        {
          TileGenerator tg;

          TileType new_tile_type = static_cast<TileType>(lua_tile_type);
          TilePtr new_tile = tg.generate(new_tile_type);

          // Copy over the common details
          new_tile->transformFrom(tile);
          map->insert(c.first, c.second, new_tile);
        }
        else
        {
          ostringstream ss;
          ss << "Null tile referenced in map_transform_tile: " << c.first << ", " << c.second;
          Log::instance().error(ss.str());
        }
      }
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to map_transform_tile");
    lua_error(ls);
  }

  lua_pushboolean(ls, result);
  return 1;
}

// Returns true if the given exit was added to the given tile, false otherwise.
int map_add_tile_exit(lua_State* ls)
{
  int result = false;

  if (lua_gettop(ls) == 5
    && lua_isstring(ls, 1)
    && lua_isnumber(ls, 2)
    && lua_isnumber(ls, 3)
    && lua_isnumber(ls, 4)
    && lua_isstring(ls, 5))
  {
    string map_id = lua_tostring(ls, 1);
    Coordinate c(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
    int lua_direction = lua_tointeger(ls, 4);
    string exit_map_id = lua_tostring(ls, 5);

    if (lua_direction >= CDIRECTION_SOUTH_WEST && lua_direction <= CDIRECTION_DOWN)
    {
      MapPtr map = Game::instance().get_map_registry_ref().get_map(map_id);

      if (map != nullptr)
      {
        MapExitUtils::add_exit_to_tile(map, c, static_cast<Direction>(lua_direction), exit_map_id);
      }
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to map_transform_tile");
    lua_error(ls);
  }

  lua_pushboolean(ls, result);
  return 1;
}
// log some text in the given log level.
// returns true if it was logged, false otherwise
// (log is not in that level, etc)
int log(lua_State* ls)
{
  int logged = false;

  if (lua_gettop(ls) == 2 && lua_isnumber(ls, 1) && lua_isstring(ls, 2))
  {
    Log& log = Log::instance();

    int log_level = lua_tointeger(ls, 1);
    string log_msg = lua_tostring(ls, 2);

    if (log_level >= LOG_LOWEST && log_level <= LOG_HIGHEST)
    {
      LoggingLevel ll = static_cast<LoggingLevel>(log_level);
      logged = log.log_using_level(ll, log_msg);
    }
  }
  else
  {
    lua_pushstring(ls, "Incorrect arguments to log");
    lua_error(ls);
  }

  lua_pushboolean(ls, logged);
  return 1;
}

// No arguments expected - ignore any.
// Returns the player's title, or an empty string if that can't be determined.
int get_player_title(lua_State* ls)
{
  string title;

  Game& game = Game::instance();
  CreaturePtr player = game.get_current_player();

  if (player)
  {
    ClassManager cm;

    title = cm.get_title(player);
  }

  lua_pushstring(ls, title.c_str());
  return 1;
}

int stop_playing_game(lua_State* ls)
{
  Game& game = Game::instance();
  game.stop_playing();

  return 0;
}