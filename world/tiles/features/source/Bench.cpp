#include "Bench.hpp"
#include "FeatureDescriptionTextKeys.hpp"

Bench::Bench()
: Feature(MaterialType::MATERIAL_TYPE_IRON, AlignmentRange::ALIGNMENT_RANGE_NEUTRAL)
{
}

Feature* Bench::clone()
{
  return new Bench(*this);
}

std::string Bench::get_description_sid() const
{
  return FeatureDescriptionTextKeys::FEATURE_DESCRIPTION_BENCH;
}

uchar Bench::get_symbol() const
{
  return '-';
}

ClassIdentifier Bench::internal_class_identifier() const
{
  return ClassIdentifier::CLASS_ID_BENCH;
}

#ifdef UNIT_TESTS
#include "unit_tests/Bench_test.cpp"
#endif

