#pragma once
#include "Controller.hpp"
#include "DecisionStrategy.hpp"

class PlayerDecisionStrategy : public DecisionStrategy
{
  public:
    PlayerDecisionStrategy(ControllerPtr new_controller);
    CommandPtr get_decision(const bool reprompt_on_cmd_not_found, const std::string& creature_id, CommandFactoryPtr command_factory, KeyboardCommandMapPtr keyboard_commands, MapPtr view_map = nullptr, int* key_p = 0);
    CommandPtr get_nonmap_decision(const bool reprompt_on_cmd_not_found, const std::string& creature_id, CommandFactoryPtr command_factory, KeyboardCommandMapPtr keyboard_commands, int* key_p = 0);
    uint get_count(const uint max_count);
    bool get_confirmation(const bool confirmation_default_value = false, const bool require_proper_selection = false) override;

    DecisionStrategy* copy();

private:
    virtual ClassIdentifier internal_class_identifier() const override;
};
