#include "gtest/gtest.h"
#include "Currency.hpp"
#include "Weapon.hpp"

TEST(SL_Engine_Calculators_ScoreCalculator, end_boss_component)
{
  Creature c;
  CreaturePtr cp(new Creature(c));

  Mortuary& m = cp->get_mortuary_ref();
  m.add_creature_kill("end_boss");

  ScoreCalculator sc;

  EXPECT_EQ(ScoreConstants::END_BOSS_BONUS, sc.calculate_score(cp));
}

TEST(SL_Engine_Calculators_ScoreCalculator, currency_component)
{
  Creature c;
  CreaturePtr cp(new Creature(c));

  CurrencyPtr currency = std::make_shared<Currency>();
  currency->set_base_id(ItemIdKeys::ITEM_ID_CURRENCY);
  currency->set_quantity(12345);

  IInventoryPtr inv = cp->get_inventory();
  inv->add(currency);

  ScoreCalculator sc;

  EXPECT_EQ(12345, sc.calculate_score(cp));
}

TEST(SL_Engine_Calculators_ScoreCalculator, experience_component)
{
  Creature c;
  CreaturePtr cp(new Creature(c));

  cp->set_experience_points(400000);

  ScoreCalculator sc;

  EXPECT_EQ(58000, sc.calculate_score(cp));

  cp->set_experience_points(1234567);

  EXPECT_EQ(129728, sc.calculate_score(cp));
}

TEST(SL_Engine_Calculators_ScoreCalculator, level_component)
{
  Creature c;
  CreaturePtr cp(new Creature(c));

  Statistic level(36);
  cp->set_level(level);

  ScoreCalculator sc;

  EXPECT_EQ(3600, sc.calculate_score(cp));
}

TEST(SL_Engine_Calculator_ScoreCalculator, artifact_component)
{
  Creature c;
  CreaturePtr cp(new Creature(c));

  WeaponPtr w1 = std::make_shared<MeleeWeapon>();
  w1->set_artifact(true);
  w1->set_value(555);

  WeaponPtr w2 = std::make_shared<MeleeWeapon>();
  w2->set_artifact(true);
  w2->set_value(445);

  IInventoryPtr inv = cp->get_inventory();
  inv->add(w1);
  cp->get_equipment().set_item(w2, EquipmentWornLocation::EQUIPMENT_WORN_WIELDED);

  ScoreCalculator sc;

  EXPECT_EQ(1000, sc.calculate_score(cp));
}

TEST(SL_Engine_Calculators_ScoreCalculator, spell_component)
{
  Creature c;
  CreaturePtr cp(new Creature(c));

  SpellKnowledge sk;
  IndividualSpellKnowledge isk;

  isk.set_castings(30);
  isk.set_bonus(5);

  sk.set_spell_knowledge("asdf", isk);

  isk.set_bonus(15);

  sk.set_spell_knowledge("aaa", isk);

  isk.set_bonus(10);

  sk.set_spell_knowledge("fdsa", isk);

  cp->set_spell_knowledge(sk);

  ScoreCalculator sc;

  EXPECT_EQ(420, sc.calculate_score(cp));
}

TEST(SL_Engine_Calculators_ScoreCalculator, total_score)
{
  Creature c;
  CreaturePtr cp(new Creature(c));

  // exp
  cp->set_experience_points(400000);

  // currency & artifacts
  CurrencyPtr currency = std::make_shared<Currency>();
  currency->set_base_id(ItemIdKeys::ITEM_ID_CURRENCY);
  currency->set_quantity(12345);

  WeaponPtr w1 = std::make_shared<MeleeWeapon>();
  w1->set_artifact(true);
  w1->set_value(555);

  WeaponPtr w2 = std::make_shared<MeleeWeapon>();
  w2->set_artifact(true);
  w2->set_value(445);

  IInventoryPtr inv = cp->get_inventory();
  inv->add(currency);
  inv->add(w1);
  cp->get_equipment().set_item(w2, EquipmentWornLocation::EQUIPMENT_WORN_WIELDED);

  // end boss
  Mortuary& m = cp->get_mortuary_ref();
  m.add_creature_kill("end_boss");

  // level
  Statistic level(36);
  cp->set_level(level);

  ScoreCalculator sc;

  int expected_score = ScoreConstants::END_BOSS_BONUS + 58000 + 12345 + 1000 + 3600;

  EXPECT_EQ(expected_score, sc.calculate_score(cp));
}
