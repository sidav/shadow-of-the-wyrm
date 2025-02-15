#include "EquipmentCommandFactory.hpp"
#include "EquipmentCommandProcessor.hpp"
#include "EquipmentKeyboardCommandMap.hpp"
#include "EquipmentManager.hpp"
#include "EquipmentScreen.hpp"
#include "EquipmentTranslator.hpp"

using namespace std;

EquipmentManager::EquipmentManager(DisplayPtr new_display, CreaturePtr new_creature)
: display(new_display), creature(new_creature)
{
}

EquipmentManager::~EquipmentManager()
{
}

// Do the actual equipment management, interfacing with the UI as needed.
ActionCostValue EquipmentManager::manage_equipment()
{
  ActionCostValue total_action_cost = ActionCostConstants::NO_ACTION;
  ActionCostValue action_cost = ActionCostConstants::NO_ACTION; // Looking at eq has no cost - adding or removing items will update this.
  ulonglong screens_created = 0;
  
  bool manage_eq = true; // For looping

  try
  {
    if (creature)
    {
      CommandFactoryPtr command_factory    = std::make_shared<EquipmentCommandFactory>();
      KeyboardCommandMapPtr kb_command_map = std::make_shared<EquipmentKeyboardCommandMap>();
    
      while (manage_eq)
      {
        if (display && creature->get_is_player())
        {
          EquipmentScreen es(display, creature);
          string screen_selection = es.display();

          if (!screen_selection.empty())
          {
            CommandPtr equipment_command = command_factory->create(screen_selection.at(0), kb_command_map->get_command_type(screen_selection));
            action_cost = EquipmentCommandProcessor::process(creature, equipment_command);

            if ((action_cost > 0) && (total_action_cost == 0))
            {
              total_action_cost = action_cost;
            }
          }
        }
        else
        {
          manage_eq = 0;
        }
      
        // -1 is a special value for the equipment command.  "0" means "don't advance the turn,
        // but keep the equipment screen up."  "-1" means "exit was selected."
        if (action_cost == -1)
        {
          manage_eq = 0;
        }
      }    
    }
  }
  catch(...)
  {
  }

  if (creature->get_is_player())
  {
    for (ulonglong i = 0; i < screens_created; i++)
    {
      display->clear_screen();
    }
  }

  return total_action_cost;
}

ActionCostValue EquipmentManager::get_action_cost_value(CreaturePtr creature) const
{
  return 1;
}
