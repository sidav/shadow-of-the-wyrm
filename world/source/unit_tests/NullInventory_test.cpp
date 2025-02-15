#include "gtest/gtest.h"
#include "Spellbook.hpp"

TEST(SW_World_NullInventory, serialization_id)
{
  NullInventory inv;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_NULL_INVENTORY, inv.get_class_identifier());
}

TEST(SW_World_NullInventory, saveload)
{
  std::shared_ptr<IInventory> inv = std::make_shared<NullInventory>();
  std::shared_ptr<IInventory> sinv2 = std::make_shared<NullInventory>();

  SpellbookPtr book = SpellbookPtr(new Spellbook());
  book->set_quantity(12);

  inv->add_front(book);

  ostringstream ss;

  inv->serialize(ss);

  istringstream iss(ss.str());

  sinv2->deserialize(iss);

  EXPECT_TRUE(*inv == *sinv2);
}

TEST(SW_World_NullInventory, items_are_not_persisted)
{
  NullInventory ni;
  SpellbookPtr book = std::make_shared<Spellbook>();
  book->set_id("book");

  ni.add_front(book);

  EXPECT_EQ(0, ni.size());
  EXPECT_TRUE(ni.empty());

  ItemPtr item = ni.get_from_base_id("book");
  
  EXPECT_TRUE(item == nullptr);
}

TEST(SW_World_NullInventory, count_items)
{
  NullInventory ni;
  EXPECT_EQ(0, ni.count_items());
}

TEST(SW_World_NullInventory, mark_is_useless)
{
  NullInventory ni;

  ni.mark_for_restack();

  EXPECT_FALSE(ni.get_marked_for_restack());
}

TEST(SW_World_NullInventory, get_allows_items)
{
  NullInventory ni;

  EXPECT_FALSE(ni.get_allows_items());
}

TEST(SW_World_NullInventory, add_items)
{
  NullInventory ni;
  ItemPtr item = std::make_shared<Spellbook>();
  ni.add_items({ item });
  EXPECT_EQ(0, ni.count_items());
}