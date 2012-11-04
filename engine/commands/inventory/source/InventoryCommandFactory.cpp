#include <boost/make_shared.hpp>
#include "InventoryCommands.hpp"
#include "InventoryCommandFactory.hpp"
#include "InventoryCommandKeys.hpp"

InventoryCommandFactory::InventoryCommandFactory()
{
}

InventoryCommandFactory::~InventoryCommandFactory()
{
}

CommandPtr InventoryCommandFactory::create(const int key, const std::string& command_name)
{
  CommandPtr inventory_command;

  if (command_name == InventoryCommandKeys::EXIT_INVENTORY)
  {
    inventory_command = boost::make_shared<ExitInventoryCommand>();
  }
  else if (command_name == InventoryCommandKeys::CLEAR_FILTER)
  {
    inventory_command = boost::make_shared<InventoryClearFilterCommand>();
  }
  else if (command_name == InventoryCommandKeys::FILTER_VIEW)
  {
    inventory_command = boost::make_shared<InventoryFilterViewCommand>();
  }
  else if (command_name == InventoryCommandKeys::NEXT_PAGE)
  {
    inventory_command = boost::make_shared<InventoryNextPageCommand>();
  }
  else if (command_name == InventoryCommandKeys::PREVIOUS_PAGE)
  {
    inventory_command = boost::make_shared<InventoryPreviousPageCommand>();
  }
  else
  {
    // If the key is in the [a-y][A-Y] range, create a selection command
    if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z'))
    {
      inventory_command = boost::make_shared<ItemSelectionCommand>(key);      
    }
  }
  
  return inventory_command;
}

CommandFactoryType InventoryCommandFactory::get_factory_type() const
{
  return COMMAND_FACTORY_TYPE_INVENTORY;
}

CommandFactory* InventoryCommandFactory::clone()
{
  return new InventoryCommandFactory(*this);
}

ClassIdentifier InventoryCommandFactory::internal_class_identifier() const
{
  return CLASS_ID_INVENTORY_COMMAND_FACTORY;
}

#ifdef UNIT_TESTS
#include "unit_tests/InventoryCommandFactory_test.cpp"
#endif
