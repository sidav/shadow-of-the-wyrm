#pragma once
#include "Directions.hpp"
#include "Map.hpp"
#include "Dimensions.hpp"
#include <set>
#include <utility>

//class MapComponents;
typedef std::vector<std::set<Coordinate> > MapComponents;
typedef std::vector<std::set<Coordinate> >::const_iterator MapComponentsItc;
typedef std::set<Coordinate> Component;
typedef std::set<Coordinate>::const_iterator ComponentItc;

class MapUtils
{
  public:    
    // Get the direction of a move given the start and end coordinates.
    static Direction get_direction(const Coordinate& start_coord, const Coordinate& end_coord);

    static std::vector<Coordinate> get_adjacent_map_coordinates(const Dimensions& dim, const int row, const int col);
    static MapComponents get_map_components(MapPtr map, const std::set<TileType>& exclusion_tiles);
    
    static bool add_or_update_location(MapPtr map, CreaturePtr creature, const Coordinate& new_coords, TilePtr creatures_old_tile = TilePtr() /* NULL by default*/);

    static TilePtr get_tile_for_creature(const MapPtr& map, const CreaturePtr& creature);
    
    static bool can_exit_map(MapExitPtr map_exit);
    
    static bool is_blocking_feature_present(TilePtr tile);
    static bool is_creature_present(TilePtr tile);
    
    static bool is_tile_available_for_creature(TilePtr tile);
    
    static Dimensions get_dimensions(MapPtr map, const Coordinate& coords, const int size);
    
    static Coordinate convert_map_key_to_coordinate(const std::string& map_key);
    
    static bool is_moving_from_land_type_tile_to_water_type_tile(TilePtr creatures_old_tile, TilePtr creatures_new_tile);
    
    static bool hostile_creature_exists(const std::string& creature_id_for_threat_check, MapPtr map);
    static bool adjacent_hostile_creature_exists(const std::string& creature_id_for_threat_check, MapPtr map);
    
  protected: 
    static void add_connected_tiles_to_component(MapPtr map, const Coordinate& coord, const Dimensions& dim, const std::set<TileType>& exclusion_tiles, Component* component);
    static bool is_tile_contained_in_an_existing_component(const Coordinate&, const MapComponents& components);
};
