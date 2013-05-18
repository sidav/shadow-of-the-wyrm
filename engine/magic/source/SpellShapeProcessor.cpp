#include <boost/foreach.hpp>
#include "ActionManager.hpp"
#include "CombatManager.hpp"
#include "CoordUtils.hpp"
#include "EffectFactory.hpp"
#include "SpellShapeProcessor.hpp"

using namespace std;

SpellShapeProcessor::~SpellShapeProcessor()
{
}

// Process the shape.  This is done by generating all the affected tiles,
// and then applying damage and spell effects to each.
bool SpellShapeProcessor::process_damage_and_effect(CreaturePtr caster, const vector<TilePtr>& affected_tiles, const Spell& spell, ActionManager * const am)
{
  // Apply the spell's damage/effect to the tiles in order.
  return apply_damage_and_effect(caster, affected_tiles, spell, am);
}

// Apply a spell to a particular tile by applying its damage and spell effect.
bool SpellShapeProcessor::apply_damage_and_effect(CreaturePtr caster, const vector<TilePtr>& affected_tiles, const Spell& spell, ActionManager * const am)
{
  bool spell_identified = false;

  BOOST_FOREACH(TilePtr tile, affected_tiles)
  {
    bool damage_identified = apply_damage(caster, tile, spell, am);
    bool effect_identified = apply_effect(caster, tile, spell, am);

    if ((damage_identified || effect_identified) && !spell_identified)
    {
      spell_identified = true;
    }
  }

  return spell_identified;
}

// Apply a spell's damage to a particular tile.
bool SpellShapeProcessor::apply_damage(CreaturePtr caster, TilePtr tile, const Spell& spell, ActionManager * const am)
{
  // A spell can be identified if the creature can see its damage type.
  // So, evoking a wand of frost, for example, identifies it.  Evoking
  // a wand with no damage and a null effect would not.
  bool spell_identified = spell.get_has_damage();

  CreaturePtr tile_creature = tile->get_creature();
  if (tile && spell.get_has_damage() && tile_creature)
  {
    CombatManager cm;

    if (spell.get_allows_bonus())
    {
      cm.attack(caster, tile_creature, ATTACK_TYPE_MAGICAL, true);
    }
    else
    {
      // If the spell doesn't allow a bonus, it's an adhoc spell not present
      // in the game's list, and so the magical damage calculator isn't
      // appropriate.  Pass the damage directly to the combat manager.
      DamagePtr dmg = boost::make_shared<Damage>(spell.get_damage());
      cm.attack(caster, tile_creature, ATTACK_TYPE_MAGICAL, false, dmg);
    }


    spell_identified = true;
  }

  return spell_identified;
}

// Apply a spell effect to a particular tile.
bool SpellShapeProcessor::apply_effect(CreaturePtr caster, TilePtr tile, const Spell& spell, ActionManager * const am)
{
  if (tile)
  {
    EffectType effect_type = spell.get_effect();

    EffectPtr effect = EffectFactory::create_effect(effect_type);
    CreaturePtr creature = tile->get_creature();

    if (effect && creature)
    {
      return effect->effect(creature, am, ITEM_STATUS_UNCURSED);
    }
  }

  // Something went wrong; anyway, the spell can't be identified in this case.
  return false;
}

bool SpellShapeProcessor::is_coordinate_adjacent_to_coordinate_in_previous_frame(const Coordinate& c, const vector<Coordinate>& prev_frame)
{
  BOOST_FOREACH(Coordinate prev_frame_coord, prev_frame)
  {
    if (CoordUtils::are_coordinates_adjacent(c, prev_frame_coord))
    {
      return true;
    }
  }

  return false;
}