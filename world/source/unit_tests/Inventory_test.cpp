#include "gtest/gtest.h"
#include "Spellbook.hpp"

TEST(SL_World_Inventory, serialization_id)
{
  Inventory inv;

  EXPECT_EQ(CLASS_ID_INVENTORY, inv.get_class_identifier());
}

TEST(SL_World_Inventory, saveload)
{
  Inventory inv, inv2;

  SpellbookPtr book = SpellbookPtr(new Spellbook());
  book->set_quantity(12);

  inv.add_front(book);

  ostringstream ss;

  inv.serialize(ss);

  istringstream iss(ss.str());

  inv2.deserialize(iss);

  EXPECT_TRUE(inv == inv2);
}

