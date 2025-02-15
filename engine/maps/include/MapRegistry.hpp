#pragma once
#include <map>
#include <string>
#include "ISerializable.hpp"
#include "Map.hpp"

using MapRegistryMap = std::map<std::string, MapPtr>;

class MapRegistry : public ISerializable
{
  public:
    MapRegistry();
    ~MapRegistry();

    bool set_map(const std::string& map_id, const MapPtr& map);
    MapPtr get_map(const std::string& map_id) const;
    bool remove_map(const std::string& map_id);
    void clear_maps();

    MapRegistryMap& get_maps_ref();

    bool serialize(std::ostream& stream) const override;
    bool deserialize(std::istream& stream) override;

  protected:
    MapRegistryMap map_registry;
    
  private:
    ClassIdentifier internal_class_identifier() const override;
};
