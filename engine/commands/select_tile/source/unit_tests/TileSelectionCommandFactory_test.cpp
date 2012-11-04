#include "gtest/gtest.h"

TEST(SL_Engine_Commands_SelectTile_TileSelectionCommandFactory, serialization_id)
{
  TileSelectionCommandFactory tscf;

  EXPECT_EQ(CLASS_ID_TILE_SELECTION_COMMAND_FACTORY, tscf.get_class_identifier());
}

