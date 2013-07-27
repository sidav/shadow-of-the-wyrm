#include "Creature.hpp"
#include "DefaultStatusEffectCalculator.hpp"
#include "Game.hpp"
#include "MessageManagerFactory.hpp"
#include "RNG.hpp"
#include "Serialize.hpp"
#include "StatusEffect.hpp"

using namespace std;

StatusEffect::StatusEffect()
{
  status_calc = boost::make_shared<DefaultStatusEffectCalculator>();
}

StatusEffect::~StatusEffect()
{
}

bool StatusEffect::should_apply_change(CreaturePtr creature) const
{
  bool status_should_apply = false;

  if (creature && !creature->has_status(get_status_identifier()) 
   && RNG::percent_chance(status_calc->calculate_pct_chance_effect(creature)))
  {
    status_should_apply = true;
  }

  return status_should_apply;
}

void StatusEffect::apply_change(CreaturePtr creature) const
{
  bool status_applied = true;

  status_applied = before_apply(creature);
  status_applied = status_applied && apply(creature);
  status_applied = status_applied && after_apply(creature);

  if (status_applied)
  {
    string message = get_player_application_message();

    if (!message.empty() && creature->get_is_player())
    {
      IMessageManager& manager = MessageManagerFactory::instance(creature);
      manager.add_new_message(message);
      manager.send();
    }
  }
}

bool StatusEffect::before_apply(CreaturePtr creature) const
{
  return true;
}

bool StatusEffect::apply(CreaturePtr creature) const
{
  if (creature)
  {
    Game& game = Game::instance();
    string status_identifier = get_status_identifier();

    double current_seconds_since_game_start = game.get_current_world()->get_calendar().get_seconds();
    int duration = status_calc->calculate_duration_in_minutes(creature);

    StatusDuration effect_duration(current_seconds_since_game_start + (duration * 60.0));

    creature->set_status(status_identifier, true);
    creature->set_status_duration(status_identifier, effect_duration);
  }

  return true;
}

bool StatusEffect::after_apply(CreaturePtr creature) const
{
  return true;
}

string StatusEffect::get_player_application_message() const
{
  string no_message;
  return no_message;
}

void StatusEffect::finalize_change(CreaturePtr creature) const
{
  before_finalize(creature);
  finalize(creature);
  after_finalize(creature);
}

void StatusEffect::before_finalize(CreaturePtr creature) const
{
}

void StatusEffect::finalize(CreaturePtr creature) const
{
  undo_change(creature);
}

string StatusEffect::get_player_finalize_message() const
{
  string no_message;
  return no_message;
}

void StatusEffect::after_finalize(CreaturePtr creature) const
{
}

void StatusEffect::undo_change(CreaturePtr creature) const
{
  before_undo(creature);
  undo(creature);
  after_undo(creature);
}

void StatusEffect::before_undo(CreaturePtr creature) const
{
}

void StatusEffect::undo(CreaturePtr creature) const
{
  if (creature)
  {
    creature->remove_status(get_status_identifier());

    if (creature->get_is_player())
    {
      IMessageManager& manager = MessageManagerFactory::instance();

      string player_undo_message = get_player_undo_message();

      if (!player_undo_message.empty())
      {
        manager.add_new_message(player_undo_message);
        manager.send();
      }
    }
  }
}

void StatusEffect::after_undo(CreaturePtr creature) const
{
}

string StatusEffect::get_player_undo_message() const
{
  string no_message;
  return no_message;
}

void StatusEffect::tick(CreaturePtr creature) const
{
}

string StatusEffect::get_status_identifier() const
{
  string no_status_identifier;
  return no_status_identifier;
}

#ifdef UNIT_TESTS
#include "unit_tests/StatusEffect_test.cpp"
#endif
