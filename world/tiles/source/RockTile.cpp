#include "RockTile.hpp"

const int RockTile::ROCK_TILE_HARDNESS = 10;

int RockTile::get_hardness() const
{
  return ROCK_TILE_HARDNESS;
}

TileType RockTile::get_tile_type() const
{
  return TileType::TILE_TYPE_ROCK;
}

std::string RockTile::get_tile_description_sid() const
{
  return TileTextKeys::TILE_DESC_ROCK;
}

int RockTile::get_movement_multiplier() const
{
  return 0;
}

Tile* RockTile::clone()
{
  return new RockTile(*this);
}
ClassIdentifier RockTile::internal_class_identifier() const
{
  return ClassIdentifier::CLASS_ID_ROCK_TILE;
}

#ifdef UNIT_TESTS
#include "unit_tests/RockTile_test.cpp"
#endif
