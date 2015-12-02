#include "MagicalCombatTargetNumberCalculator.hpp"

MagicalCombatTargetNumberCalculator::MagicalCombatTargetNumberCalculator()
: CombatTargetNumberCalculator(AttackType::ATTACK_TYPE_MAGICAL)
{
}

// The target number for magical combat is the attacked creature's evade minus half the attacker's Magic skill.
//
// That is, magic should be very, very easy to hit with.
int MagicalCombatTargetNumberCalculator::calculate(CreaturePtr attacking_creature, CreaturePtr attacked_creature)
{
  int target_number = 0;

  if (attacking_creature && attacked_creature)
  {
    int defender_evade = attacked_creature->get_evade().get_current();
    int attacker_magic_value = (attacking_creature->get_skills().get_skill(SkillType::SKILL_GENERAL_MAGIC)->get_value() / 2);

    target_number = defender_evade - attacker_magic_value;

    if (target_number < 0) target_number = 0;
  }

  return target_number;
}

int MagicalCombatTargetNumberCalculator::calculate_pct_chance_pass_through_untargetted_square(CreaturePtr attacking_creature, CreaturePtr defending_creature)
{
  return 100;
}
