#include "MagicToHitCalculator.hpp"
#include "SkillManager.hpp"
#include "CarryingCapacityCalculator.hpp"

const int MagicToHitCalculator::MAGIC_WEIGHT_DIVISOR = 2;

MagicToHitCalculator::MagicToHitCalculator()
: ToHitCalculator(AttackType::ATTACK_TYPE_MAGICAL)
{
}

int MagicToHitCalculator::calculate(CreaturePtr creature)
{
  int to_hit = 0;

  if (creature)
  {
    to_hit = creature->get_to_hit().get_current();

    SkillManager sm;

    // Have you ever tried directing a fireball while drunk?
    // It's not easy!
    int bac_modifier = static_cast<int>(creature->get_blood().get_blood_alcohol_content() * 100);
    int magic_bonus  = sm.get_skill_value(creature, SkillType::SKILL_GENERAL_MAGIC) / NWP_SKILL_BONUS_DIVISOR;
    int intelligence = creature->get_intelligence().get_current();

    // Harder to wiggle fingers and nose while schlepping fifty broadswords.
    int creature_carried_weight = creature->get_weight_carried();

    CarryingCapacityCalculator ccc;
    int burdened_limit = ccc.calculate_burdened_weight(creature);
    int attacker_weight_value = 0;

    if (creature_carried_weight > burdened_limit)
    {
      attacker_weight_value = static_cast<int>((creature_carried_weight - burdened_limit) / 16 /* oz to lbs */ / MAGIC_WEIGHT_DIVISOR);
    }

    to_hit += magic_bonus;
    to_hit += (intelligence - 10) / 3;
    to_hit -= bac_modifier;
    to_hit -= attacker_weight_value;
  }

  return to_hit;
}

#ifdef UNIT_TESTS
#include "unit_tests/MagicToHitCalculator_test.cpp"
#endif
