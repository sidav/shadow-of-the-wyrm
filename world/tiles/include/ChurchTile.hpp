#pragma once
#include "WorshipSiteTile.hpp"

class ChurchTile : public WorshipSiteTile
{
  public:    
    ChurchTile();
    ChurchTile(const TileType subtype);

    TileType get_tile_type() const;

    std::string get_tile_description_sid() const;

    virtual Tile* clone();

  private:
    virtual ClassIdentifier internal_class_identifier() const;
};
