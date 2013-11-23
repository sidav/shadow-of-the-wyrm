#pragma once
#include <string>
#include <map>
#include <vector>
#include <boost/unordered_map.hpp>
#include "common.hpp"
#include "Dimensions.hpp"
#include "ISerializable.hpp"
#include "Tile.hpp"
#include "MapExit.hpp"
#include "MapTypes.hpp"

// Forward declarations.
class Creature;
class Tile;

typedef boost::unordered_map<std::string, std::shared_ptr<Tile>> TilesContainer;

class Map : public ISerializable
{
	public:
    Map(const Map& new_map);
		Map(const Dimensions& new_dimensions, const Dimensions& original_dimensions = Dimensions());
    bool operator==(const Map& map) const;
		
		// Functions used when creating a view map - reset the
		// creatures and locations after copying the map.
		void reset_creatures_and_locations();

		// These functions work on a list built up from the map itself.
		// This allows for quicker lookup.
		bool has_creature(const std::string& creature_id);
		std::shared_ptr<Creature> get_creature(const std::string& creature_id);
		std::map<std::string, std::shared_ptr<Creature>> get_creatures();
		std::map<std::string, std::shared_ptr<Creature>>& get_creatures_ref();
    void add_creature(std::shared_ptr<Creature> creature);
		void remove_creature(const std::string& creature_id);
		// Other get_creature... fns here.
		// Other remove_creature... fns here.

    // Shouldn't generally be called - used by the XML readers to directly set
    // the tiles.
    void set_tiles(const TilesContainer& new_tiles);

		bool insert(int row, int col, std::shared_ptr<Tile> tile);
		std::shared_ptr<Tile> at(int row, int col);
		std::shared_ptr<Tile> at(const Coordinate& c);

		void set_size(const Dimensions& new_dimensions);
		Dimensions size() const;
		
		// If the map is not a view map, this will return the same as
		// size().  If the map is a view map, this will return the map's
		// original dimensions, as if it were a "regular" map.
    void set_original_size(const Dimensions& new_original_dimensions);
		Dimensions original_size() const;
		
		void set_terrain_type(const TileType new_terrain_type);
		TileType get_terrain_type() const;

		void set_map_type(const MapType& new_type);
		MapType get_map_type() const;
		
		TilesContainer get_tiles() const;

    void clear_locations();
    void add_or_update_location(const std::string& location, const Coordinate& coordinate);
    void remove_location(const std::string& location);
    bool has_location(const std::string& location);
    Coordinate get_location(const std::string& location) const;
    std::shared_ptr<Tile> get_tile_at_location(const std::string& location);
    
    void set_map_exit(MapExitPtr new_map_exit);
    MapExitPtr get_map_exit() const;
    
    void set_map_id(const std::string& new_map_id);
    std::string get_map_id() const;
    
    std::string get_map_exit_id() const;
    
    void set_permanent(const bool permenance);
    bool get_permanent() const;

    // Set/get the base danger level.  This is used to determine the danger of the monsters to
    // generate, as well as the quality of the items.
    void set_danger(const uint new_danger);
    uint get_danger() const;

    bool serialize(std::ostream& stream);
    bool deserialize(std::istream& stream);

    static TileKey make_map_key(const int row, const int col);

		void create_creatures();		

	protected:
    friend class SL_Engine_Map;
    
    // NOTE: This information is also stored at the Tile level, but since it's a shared_ptr, that's okay.
    // Ensure that when creatures are created or killed, both data structures are updated accordingly.
    std::map<std::string, std::shared_ptr<Creature>> creatures;

	TilesContainer tiles;
	Dimensions dimensions;
	Dimensions original_dimensions;
    NamedMapLocations locations;
    TileType terrain_type; // field, forest, village, etc.
    MapType map_type;
    MapExitPtr map_exit;
    std::string map_id;
    bool permanent;
    uint danger;

  private:
    ClassIdentifier internal_class_identifier() const;
};

typedef std::shared_ptr<Map> MapPtr;
