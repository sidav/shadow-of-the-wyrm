#pragma once
#include "KeyboardCommandMap.hpp"

class InventoryKeyboardCommandMap : public KeyboardCommandMap
{
  public:
    InventoryKeyboardCommandMap();
    virtual ~InventoryKeyboardCommandMap();

    // Fail silently
    virtual void command_not_found(const std::string& keyboard_input) override;

    // serialize/deserialize taken care of by KeyboardCommandMap

    virtual KeyboardCommandMap* clone() override;

  protected:
    // Command mapping for keys on the inventory screen
    virtual void initialize_command_mapping() override;

  private:
    ClassIdentifier internal_class_identifier() const override;
};
