#include <iterator>
#include <map>
#include "Conversion.hpp"
#include "CreatureCalculator.hpp"
#include "CreatureGenerationConstants.hpp"
#include "CreatureGenerationManager.hpp"
#include "CreatureFactory.hpp"
#include "Game.hpp"
#include "MapProperties.hpp"
#include "RaceManager.hpp"
#include "RNG.hpp"

using namespace std;

CreatureGenerationManager::CreatureGenerationManager()
{
}

CreatureGenerationMap CreatureGenerationManager::generate_creature_generation_map(const TileType map_terrain_type, const bool permanent_map, const int min_danger_level, const int max_danger_level, const Rarity rarity, const map<string, string>& additional_properties)
{
  int min_danger = min_danger_level;
  CreatureGenerationMap generation_map;

  CreaturePtr generated_creature;
  Game& game = Game::instance();
  
  CreatureMap creatures = game.get_creatures_ref();
  CreatureGenerationValuesMap cgv_map = game.get_creature_generation_values_ref();

  bool ignore_level_checks = false;

  auto a_it = additional_properties.find(MapProperties::MAP_PROPERTIES_IGNORE_CREATURE_LVL_CHECKS);
  if (a_it != additional_properties.end())
  {
    ignore_level_checks = String::to_bool(a_it->second);
  }

  // If generation needs to be restricted to undead, dragons, etc.
  string required_race;
  a_it = additional_properties.find(MapProperties::MAP_PROPERTIES_GENERATED_CREATURE_RACE_ID);
  if (a_it != additional_properties.end())
  {
    required_race = a_it->second;
  }

  // If generation needs to be restricted to one of a number of creature IDs.
  vector<string> preset_creature_ids;
  a_it = additional_properties.find(MapProperties::MAP_PROPERTIES_CREATURE_IDS);
  if (a_it != additional_properties.end())
  {
    preset_creature_ids = String::create_string_vector_from_csv_string(a_it->second);
  }

  vector<string> generator_filters;
  a_it = additional_properties.find(MapProperties::MAP_PROPERTIES_GENERATOR_FILTERS);

  if (a_it != additional_properties.end())
  {
    generator_filters = String::create_string_vector_from_csv_string(a_it->second);

    // Sort this for easy comparison later on...
    std::sort(generator_filters.begin(), generator_filters.end());
  }
    
  while (generation_map.empty() && (min_danger > 0))
  {
    // Build the map of creatures available for generation given the danger level and rarity
    for (CreatureMap::iterator c_it = creatures.begin(); c_it != creatures.end(); c_it++)
    {
      string creature_id = c_it->first;

      CreaturePtr creature = c_it->second;
      CreatureGenerationValues cgvals = cgv_map[creature_id];

      if (does_creature_match_generation_criteria(cgvals, map_terrain_type, permanent_map, min_danger, max_danger_level, rarity, ignore_level_checks, required_race, generator_filters, preset_creature_ids))
      {
        generation_map.insert(make_pair(creature_id, make_pair(creature, cgvals)));
      }
    }

    min_danger /= 2;
  }
  
  return generation_map;
}

CreaturePtr CreatureGenerationManager::generate_creature(ActionManager& am, CreatureGenerationMap& generation_map)
{
  CreaturePtr generated_creature;
  
  // Iterate through the generation map, and attempt to generate a creature with probability P,
  // where P = (danger level / danger_level + num_creatures_in_map)
  int p_denominator = 0;
    
  // Get the denominator for the probabilistic generation by summing the danger level over all creatures
  // in the map.
  for(CreatureGenerationMap::iterator c_it = generation_map.begin(); c_it != generation_map.end(); c_it++)
  {
    CreatureGenerationValues cgv = c_it->second.second;
    p_denominator += cgv.get_danger_level();
  }
    
  float p_denominator_f = static_cast<float>(p_denominator);

  // Determine the creature to generate
  for(CreatureGenerationMap::iterator c_it = generation_map.begin(); c_it != generation_map.end(); c_it++)
  {
    CreatureGenerationValues cgv = c_it->second.second;

    int p_numerator = cgv.get_danger_level();
    int P = static_cast<int>((static_cast<float>(p_numerator) / p_denominator_f) * 100);
      
    // Generate the creature if we hit the percentage, or if we're on the last item in the map
    // and a creature has not yet been generated.
    if (!cgv.is_maximum_reached() && (RNG::percent_chance(P) || ((distance(c_it, generation_map.end()) == 1) && !generated_creature)))
    {
      string creature_id = c_it->first;
      CreatureFactory cf;
      generated_creature = cf.create_by_creature_id(am, creature_id);

      break;
    }
  }
   
  if (generated_creature)
  {
    CreatureCalculator::update_calculated_values(generated_creature);
  }
    
  return generated_creature;
}

bool CreatureGenerationManager::does_creature_match_generation_criteria(const CreatureGenerationValues& cgv, const TileType terrain_type, const bool permanent_map, const int min_danger_level, const int max_danger_level, const Rarity rarity, const bool ignore_level_checks, const string& required_race, const vector<string>& generator_filters, const vector<string>& preset_creature_ids)
{
  RaceManager rm;
  int cgv_danger_level = cgv.get_danger_level();
  int cgv_maximum = cgv.get_maximum();
  vector<string> cgv_generator_filters = cgv.get_generator_filters();

  // Sort the vectors.  The assumption is that the input vector is sorted.
  std::sort(cgv_generator_filters.begin(), cgv_generator_filters.end());

  if ( cgv.is_terrain_type_allowed(terrain_type)
    && cgv_danger_level >= 0 // Exclude danger level of -1, which means "don't generate"
    && (ignore_level_checks || cgv_danger_level >= min_danger_level)
    && (ignore_level_checks || cgv_danger_level <= max_danger_level)
    && (required_race.empty() || rm.is_race_or_descendent(cgv.get_race_id(), required_race))
    && (cgv_maximum <= CreatureGenerationConstants::CREATURE_GENERATION_UNLIMITED || (cgv.get_current() < cgv_maximum)) // Either no max, or less than the > 0 maximum
    // If a list of preset creature IDs are present, filter out all creature
    // IDs not in that list.
    && (preset_creature_ids.empty() || (std::find(preset_creature_ids.begin(), preset_creature_ids.end(), cgv.get_id()) != preset_creature_ids.end()))
    && (cgv_maximum != 1 || permanent_map) // no uniques on temporary maps
    // If there are no generator filters (from the generator/map) the creature
    // can be generated.  If there are generator filters, then the creature's
    // values must be present in the generator/map's.
    && (generator_filters.empty() || std::includes(cgv_generator_filters.begin(), cgv_generator_filters.end(), generator_filters.begin(), generator_filters.end()))
    && cgv.get_rarity() <= rarity )
  {
    return true;
  }
  
  return false;
}
