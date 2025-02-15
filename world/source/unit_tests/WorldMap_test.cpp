#include "gtest/gtest.h"
#include "FieldGenerator.hpp"

TEST(SW_World_Map, serialization_id)
{
  Dimensions d;
  Map map(d);

  EXPECT_EQ(ClassIdentifier::CLASS_ID_MAP, map.get_class_identifier());
}

TEST(SW_World_Map, saveload)
{
  MapPtr map, map2;
  Dimensions d;

  FieldGenerator field_gen("test");
  map = field_gen.generate(d);
  map2 = MapPtr(new Map(d));

  ostringstream ss;

  map->serialize(ss);

  istringstream iss(ss.str());

  map2->deserialize(iss);

  EXPECT_TRUE(*map == *map2);
}

