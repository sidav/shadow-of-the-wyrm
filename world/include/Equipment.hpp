#pragma once
#include <map>
#include "EquipmentTypes.hpp"
#include "Item.hpp"
#include "ISerializable.hpp"

using EquipmentMap = std::map<EquipmentWornLocation, ItemPtr>;

// Equipment is a class that represents all the "worn" equipment of a particular creature -
// the items that are not carried as inventory.
class Equipment : public ISerializable
{
  public:
    Equipment();
    ~Equipment();
    bool operator==(const Equipment& e) const;
    
    bool set_item(ItemPtr item, const EquipmentWornLocation location);
    ItemPtr get_item(const EquipmentWornLocation location) const;
    ItemPtr remove_item(const EquipmentWornLocation location);
    uint count_items() const;

    // Used to determine if a single item must be equipped (most slots), or if
    // multiple items are allowed (ammunition).
    bool can_equip_multiple_items(const EquipmentWornLocation location) const;
    
    bool merge(ItemPtr item);
    
    EquipmentMap get_equipment() const;

    virtual bool serialize(std::ostream& stream) const override;
    virtual bool deserialize(std::istream& stream) override;

  protected:
    void initialize();
    EquipmentMap equipment;

  private:
    virtual ClassIdentifier internal_class_identifier() const override;
};
