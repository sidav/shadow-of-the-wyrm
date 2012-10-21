#pragma once
#include "IDescriber.hpp"
#include "Creature.hpp"

class CreatureDescriber : public IDescriber
{
  public:
    CreatureDescriber(CreaturePtr new_creature);

    std::string describe() const;

    std::string describe_for_save_file() const;

  protected:
    CreaturePtr creature;
};
