#include "Map.hpp"

using namespace std;

Map::Map(const Map& new_map)
: map_type(MAP_TYPE_OVERWORLD)
{
  if (this != &new_map)
  {
    Dimensions new_dimensions = new_map.size();
    std::map<std::pair<int, int>, TilePtr > new_tiles = new_map.get_tiles();

    dimensions = new_dimensions;
    tiles = new_tiles;
  }
}

Map::Map(const Dimensions& new_dimensions)
: map_type(MAP_TYPE_OVERWORLD)
{
  dimensions = new_dimensions;
}

// Create the creature list by iterating over all the map tiles, and adding any Creature
// attached to a tile.
void Map::create_creatures()
{
  creatures.clear();

  std::map<Coordinate, TilePtr >::iterator map_it;

  for (map_it = tiles.begin(); map_it != tiles.end(); map_it++)
  {
    TilePtr current_tile = map_it->second;

    CreaturePtr potential_creature = current_tile->get_creature();

    if (potential_creature)
    {
      creatures.push_back(potential_creature);
    }
  }
}

CreaturePtr Map::get_creature(const uint idx)
{
  CreaturePtr creature;

  if (idx < creatures.size())
  {
    return creatures.at(idx);
  }

  return creature;
}

vector<CreaturePtr> Map::get_creatures()
{
  if (creatures.empty())
  {
    create_creatures();
  }

  return creatures;
}

bool Map::insert(int row, int col, TilePtr tile)
{
  pair<int, int> key = make_pair(row, col);

  tiles[key] = tile;
  return true;
}

TilePtr Map::at(int row, int col)
{
  pair<int, int> key = make_pair(row, col);
  return tiles[key];
}

void Map::set_size(const Dimensions& new_dimensions)
{
  dimensions = new_dimensions;
}

Dimensions Map::size() const
{
  return dimensions;
}

void Map::set_map_type(const MapType& new_map_type)
{
  map_type = new_map_type;
}

MapType Map::get_map_type() const
{
  return map_type;
}

void Map::set_parent_map(MapPtr new_parent_map)
{
  parent_map = new_parent_map;
}

MapPtr Map::get_parent_map() const
{
  return parent_map;
}

void Map::set_child_map(MapPtr new_child_map)
{
  child_map = new_child_map;
}

MapPtr Map::get_child_map() const
{
  return child_map;
}

std::map<std::pair<int, int>, TilePtr > Map::get_tiles() const
{
  return tiles;
}

void Map::clear_locations()
{
  locations.clear();
}

void Map::add_or_update_location(const string& location, const Coordinate& coordinate)
{
  locations[location] = coordinate;
}

Coordinate Map::get_location(const string& location) const
{
  Coordinate c;

  c.first = 0;
  c.second = 0;

  NamedMapLocations::const_iterator l_it = locations.find(location);

  if (l_it != locations.end())
  {
    c = l_it->second;
  }

  return c;
}

TilePtr Map::get_tile_at_location(const string& location)
{
  TilePtr tile;

  NamedMapLocations::iterator n_it = locations.find(location);

  if (n_it != locations.end())
  {
    Coordinate coord = n_it->second;
    int row = coord.first;
    int col = coord.second;

    tile = at(row, col);
  }

  return tile;
}
