#pragma once
#include <map>
#include <string>
#include "common.hpp"

enum struct MapType
{
  MAP_TYPE_WORLD = 0
, MAP_TYPE_OVERWORLD = 1
, MAP_TYPE_UNDERWORLD = 2
, MAP_TYPE_UNDERWATER = 3
};

class MapID
{
  public:
    static const std::string MAP_ID_WORLD_MAP;

  protected:
    MapID();
};

using NamedMapLocations = std::map<std::string, Coordinate>;

