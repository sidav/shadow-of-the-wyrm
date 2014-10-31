#include "IFeatureManipulatorFactory.hpp"
#include "DefaultFeatureManipulator.hpp"
#include "PewManipulator.hpp"
#include "GoodAltarManipulator.hpp"
#include "NeutralAltarManipulator.hpp"
#include "EvilAltarManipulator.hpp"
#include "DoorGateManipulator.hpp"
#include "BarrelManipulator.hpp"

IFeatureManipulatorPtr IFeatureManipulatorFactory::create_manipulator(FeaturePtr feature)
{
  IFeatureManipulatorPtr manipulator;

  if (feature != nullptr)
  {
    ClassIdentifier class_id = feature->get_class_identifier();

    if (class_id == CLASS_ID_PEW)
    {
      manipulator = std::make_shared<PewManipulator>(feature);
    }
    else if (class_id == CLASS_ID_GOOD_ALTAR)
    {
      manipulator = std::make_shared<GoodAltarManipulator>(feature);
    }
    else if (class_id == CLASS_ID_NEUTRAL_ALTAR)
    {
      manipulator = std::make_shared<NeutralAltarManipulator>(feature);
    }
    else if (class_id == CLASS_ID_EVIL_ALTAR)
    {
      manipulator = std::make_shared<EvilAltarManipulator>(feature);
    }
    else if (class_id == CLASS_ID_DOOR || class_id == CLASS_ID_GATE)
    {
      manipulator = std::make_shared<DoorGateManipulator>(feature);
    }
    else if (class_id == CLASS_ID_BARREL)
    {
      manipulator = std::make_shared<BarrelManipulator>(feature);
    }
    // All other features
    else
    {
      manipulator = std::make_shared<DefaultFeatureManipulator>(feature);
    }
  }

  return manipulator;
}