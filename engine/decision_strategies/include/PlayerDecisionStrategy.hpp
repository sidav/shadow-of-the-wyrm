#pragma once
#include "Controller.hpp"
#include "DecisionStrategy.hpp"

class PlayerDecisionStrategy : public DecisionStrategy
{
  public:
    PlayerDecisionStrategy(ControllerPtr new_controller);
    CommandPtr get_decision(const std::string& creature_id, CommandFactoryPtr command_factory, KeyboardCommandMapPtr keyboard_commands, MapPtr view_map = MapPtr());
    uint get_count(const uint max_count);
    bool get_confirmation();

    DecisionStrategy* copy();

  protected:
    ControllerPtr controller;
};
