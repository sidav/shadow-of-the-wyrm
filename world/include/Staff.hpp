#pragma once
#include "Item.hpp"

class Staff : public Item
{
  public:
    Staff();
    ~Staff();
    
    Item* clone() override;

  private:
    ClassIdentifier internal_class_identifier() const override;
};

using StaffPtr = std::shared_ptr<Staff>;
