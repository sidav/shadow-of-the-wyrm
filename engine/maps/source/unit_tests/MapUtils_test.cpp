#include "gtest/gtest.h"
#include "GeneratorUtils.hpp"
#include "TileGenerator.hpp"

TEST(SW_Engine_Maps_MapUtils, get_dimensions)
{
  // Size = 0 - dimensions should be 0, regardless.
  uint size = 0;
  Dimensions d;
  int max_rows = d.get_y();
  int max_cols = d.get_x();
  
  Coordinate c(0, 0);
  MapPtr original_map = std::make_shared<Map>(d);

  // "1" is the row/col on which the creature currently stands.  This is always visible.
  EXPECT_EQ(1, MapUtils::get_dimensions(original_map, c, size).get_y());
  EXPECT_EQ(1, MapUtils::get_dimensions(original_map, c, size).get_x());
  
  c.first = 5;
  c.second = 5;
  
  EXPECT_EQ(1, MapUtils::get_dimensions(original_map, c, size).get_y());
  EXPECT_EQ(1, MapUtils::get_dimensions(original_map, c, size).get_x());

  // Centre of a map where the size can expand in all directions
  size = 7;
  c.first = 10;
  c.second = 40;

  int expected_size = size+size+1; // +1 is the creature's row.  visibility extends in each direction.
  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_y());
  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_x());

  // Edge cases
  // Size 5, at 0,0
  size = 5;
  c.first = 0;
  c.second = 0;
  
  expected_size = size+1;

  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_y());
  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_x());
  
  // Size 5, at 0, max
  c.first = 0;
  c.second = max_cols-1;

  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_y());
  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_x());

  // Size 5, at max, 0
  c.first = max_rows-1;
  c.second = 0;

  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_y());
  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_x());

  // Size 5, at max, max
  c.first = max_rows-1;
  c.second = max_cols-1;

  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_y());
  EXPECT_EQ(expected_size, MapUtils::get_dimensions(original_map, c, size).get_x());
}

TEST(SW_Engine_Maps_MapUtils, coordinate_within_dimensions)
{
  Dimensions d;
  Coordinate c(-1, -1);

  EXPECT_FALSE(MapUtils::are_coordinates_within_dimensions(c, d));

  c.first = 0;
  
  EXPECT_FALSE(MapUtils::are_coordinates_within_dimensions(c, d));

  c.second = 0;

  EXPECT_TRUE(MapUtils::are_coordinates_within_dimensions(c, d));

  c.first = d.get_y() - 1;
  c.second = d.get_x() - 1;

  EXPECT_TRUE(MapUtils::are_coordinates_within_dimensions(c, d));

  c.first++;

  EXPECT_FALSE(MapUtils::are_coordinates_within_dimensions(c, d));

  c.first--;
  c.second++;

  EXPECT_FALSE(MapUtils::are_coordinates_within_dimensions(c, d));
}

TEST(SW_Engine_Maps_MapUtils, calculate_depth_delta_map)
{
  EXPECT_EQ(1, MapUtils::calculate_depth_delta(nullptr, nullptr, ExitMovementType::EXIT_MOVEMENT_DESCEND));
  EXPECT_EQ(-1, MapUtils::calculate_depth_delta(nullptr, nullptr, ExitMovementType::EXIT_MOVEMENT_ASCEND));

  Dimensions dim;
  MapPtr map = std::make_shared<Map>(dim);

  EXPECT_EQ(1, MapUtils::calculate_depth_delta(map, nullptr, ExitMovementType::EXIT_MOVEMENT_DESCEND));
  EXPECT_EQ(-1, MapUtils::calculate_depth_delta(map, nullptr, ExitMovementType::EXIT_MOVEMENT_ASCEND));

  map->set_property(TileProperties::TILE_PROPERTY_DEPTH_INCREMENT, "5");

  EXPECT_EQ(5, MapUtils::calculate_depth_delta(map, nullptr, ExitMovementType::EXIT_MOVEMENT_DESCEND));
  EXPECT_EQ(-5, MapUtils::calculate_depth_delta(map, nullptr, ExitMovementType::EXIT_MOVEMENT_ASCEND));
}

TEST(SW_Engine_Maps_MapUtils, calculate_depth_delta_tile)
{
  EXPECT_EQ(1, MapUtils::calculate_depth_delta(nullptr, nullptr, ExitMovementType::EXIT_MOVEMENT_DESCEND));
  EXPECT_EQ(-1, MapUtils::calculate_depth_delta(nullptr, nullptr, ExitMovementType::EXIT_MOVEMENT_ASCEND));

  Dimensions dim;
  TilePtr tile = std::make_shared<FieldTile>();

  EXPECT_EQ(1, MapUtils::calculate_depth_delta(nullptr, tile, ExitMovementType::EXIT_MOVEMENT_DESCEND));
  EXPECT_EQ(-1, MapUtils::calculate_depth_delta(nullptr, tile, ExitMovementType::EXIT_MOVEMENT_ASCEND));

  tile->set_additional_property(TileProperties::TILE_PROPERTY_DEPTH_INCREMENT, "5");

  EXPECT_EQ(5, MapUtils::calculate_depth_delta(nullptr, tile, ExitMovementType::EXIT_MOVEMENT_DESCEND));
  EXPECT_EQ(-5, MapUtils::calculate_depth_delta(nullptr, tile, ExitMovementType::EXIT_MOVEMENT_ASCEND));
}

TEST(SW_Engine_Maps_MapUtils, should_link_entry_point)
{
  EXPECT_FALSE(MapUtils::should_link_entry_point(MapType::MAP_TYPE_OVERWORLD));
  EXPECT_TRUE(MapUtils::should_link_entry_point(MapType::MAP_TYPE_WORLD));
  EXPECT_TRUE(MapUtils::should_link_entry_point(MapType::MAP_TYPE_UNDERWORLD));
  EXPECT_TRUE(MapUtils::should_link_entry_point(MapType::MAP_TYPE_UNDERWATER));
  EXPECT_TRUE(MapUtils::should_link_entry_point(MapType::MAP_TYPE_COSMOS));
}

TEST(SW_Engine_Maps_MapUtils, does_area_around_tile_contain_staircase)
{
  Dimensions dim;
  MapPtr map = std::make_shared<Map>(dim);
  TileGenerator tg;

  GeneratorUtils::fill(map, make_pair(0,0), make_pair(dim.get_y()-1, dim.get_x()-1), TileType::TILE_TYPE_DUNGEON);

  // Put an up staircase at 4,4 and a down staircase at 15,15
  TilePtr up_staircase = tg.generate_staircase(StaircaseType::STAIRCASE_UP);
  TilePtr down_staircase = tg.generate_staircase(StaircaseType::STAIRCASE_DOWN);

  map->insert(make_pair(4,4), up_staircase);

  EXPECT_TRUE(MapUtils::does_area_around_tile_contain_staircase(map, make_pair(4,5)));
  EXPECT_TRUE(MapUtils::does_area_around_tile_contain_staircase(map, make_pair(5,4)));
  EXPECT_FALSE(MapUtils::does_area_around_tile_contain_staircase(map, make_pair(7,7)));

  map->insert(make_pair(15,15), down_staircase);

  EXPECT_TRUE(MapUtils::does_area_around_tile_contain_staircase(map, make_pair(15,16)));
  EXPECT_TRUE(MapUtils::does_area_around_tile_contain_staircase(map, make_pair(14,15)));
  EXPECT_FALSE(MapUtils::does_area_around_tile_contain_staircase(map, make_pair(10,10)));
}

TEST(SW_Engine_Maps_MapUtils, is_coordinate_within_player_restricted_zone)
{
  Dimensions dim;
  MapPtr map = std::make_shared<Map>(dim);

  GeneratorUtils::fill(map, make_pair(0,0), make_pair(dim.get_y()-1, dim.get_x()-1), TileType::TILE_TYPE_DUNGEON);

  map->add_or_update_location(CreatureID::CREATURE_ID_PLAYER, make_pair(0,0));
  Coordinate c = make_pair(7,7);

  EXPECT_TRUE(MapUtils::is_coordinate_within_player_restricted_zone(map, c));

  c = make_pair(8,8);

  EXPECT_TRUE(MapUtils::is_coordinate_within_player_restricted_zone(map, c));

  c = make_pair(9,9);

  EXPECT_FALSE(MapUtils::is_coordinate_within_player_restricted_zone(map, c));
}

TEST(SW_Engine_Maps_MapUtils, get_map_exit_direction)
{
  Dimensions dim;

  // NW corner
  Coordinate c = { 0,0 };
  EXPECT_EQ(Direction::DIRECTION_NORTH_WEST, MapUtils::get_exit_direction(Direction::DIRECTION_NORTH_WEST, dim, c));

  // NE corner
  c = { 0, 79 };
  EXPECT_EQ(Direction::DIRECTION_NORTH_EAST, MapUtils::get_exit_direction(Direction::DIRECTION_NORTH_EAST, dim, c));

  // SW corner
  c = { 19,0 };
  EXPECT_EQ(Direction::DIRECTION_SOUTH_WEST, MapUtils::get_exit_direction(Direction::DIRECTION_SOUTH_WEST, dim, c));

  // SE corner
  c = { 19,79 };
  EXPECT_EQ(Direction::DIRECTION_SOUTH_EAST, MapUtils::get_exit_direction(Direction::DIRECTION_SOUTH_EAST, dim, c));

  // Other cases
  c = { 0,5 };
  EXPECT_EQ(Direction::DIRECTION_NORTH, MapUtils::get_exit_direction(Direction::DIRECTION_NORTH, dim, c));
  EXPECT_EQ(Direction::DIRECTION_NORTH, MapUtils::get_exit_direction(Direction::DIRECTION_NORTH_WEST, dim, c));
  EXPECT_EQ(Direction::DIRECTION_NORTH, MapUtils::get_exit_direction(Direction::DIRECTION_NORTH_EAST, dim, c));
  
  c = { 5, 0 };
  EXPECT_EQ(Direction::DIRECTION_WEST, MapUtils::get_exit_direction(Direction::DIRECTION_WEST, dim, c));
  EXPECT_EQ(Direction::DIRECTION_WEST, MapUtils::get_exit_direction(Direction::DIRECTION_NORTH_WEST, dim, c));
  EXPECT_EQ(Direction::DIRECTION_WEST, MapUtils::get_exit_direction(Direction::DIRECTION_SOUTH_WEST, dim, c));

  c = { 5, 79 };

  EXPECT_EQ(Direction::DIRECTION_EAST, MapUtils::get_exit_direction(Direction::DIRECTION_EAST, dim, c));
  EXPECT_EQ(Direction::DIRECTION_EAST, MapUtils::get_exit_direction(Direction::DIRECTION_NORTH_EAST, dim, c));
  EXPECT_EQ(Direction::DIRECTION_EAST, MapUtils::get_exit_direction(Direction::DIRECTION_SOUTH_EAST, dim, c));
  
  c = { 19, 5 };

  EXPECT_EQ(Direction::DIRECTION_SOUTH, MapUtils::get_exit_direction(Direction::DIRECTION_SOUTH, dim, c));
  EXPECT_EQ(Direction::DIRECTION_SOUTH, MapUtils::get_exit_direction(Direction::DIRECTION_SOUTH_WEST, dim, c));
  EXPECT_EQ(Direction::DIRECTION_SOUTH, MapUtils::get_exit_direction(Direction::DIRECTION_SOUTH_EAST, dim, c));
}
