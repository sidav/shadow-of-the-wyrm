#pragma once
#include "Feature.hpp"

// This is a "north-south" pew: the symbol is '-'.
// "east-west" pews are defined in EastWestPew.hpp.
class Pew : public Feature
{
  public:
    Pew();
    virtual Feature* clone() override;

    virtual uchar get_symbol() const override;
    virtual float get_piety_loss_multiplier() const override;

  protected:
    virtual std::string get_description_sid() const override;

  private:
    virtual ClassIdentifier internal_class_identifier() const override;
};
