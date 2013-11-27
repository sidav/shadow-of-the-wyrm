#pragma once
#include "StatusEffect.hpp"

class DisfiguredStatusEffect : public StatusEffect
{
  public:
    DisfiguredStatusEffect();
  
  protected:
    virtual bool after_apply(std::shared_ptr<Creature> creature) const override;
    virtual void after_undo(std::shared_ptr<Creature> creature) const override;

    virtual std::string get_player_application_message() const override;
    virtual std::string get_player_undo_message() const override;
    virtual std::string get_npc_application_message(std::shared_ptr<Creature> creature) const override;
    virtual std::string get_npc_undo_message(std::shared_ptr<Creature> creature) const override;

    virtual std::string get_status_identifier() const override;
};

