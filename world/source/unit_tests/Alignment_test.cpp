#include "gtest/gtest.h"

TEST(SL_World_Alignment, set_values)
{
  Alignment a;

  EXPECT_EQ(0, a.get_alignment());

  a.set_alignment(1200);

  EXPECT_EQ(1200, a.get_alignment());

  a.set_alignment(26000);

  // Can't set above the range.
  EXPECT_EQ(1200, a.get_alignment());

  a.set_alignment(-1250);

  EXPECT_EQ(-1250, a.get_alignment());

  a.set_alignment(-3500);

  // Can't set below the range, either.
  EXPECT_EQ(-1250, a.get_alignment());
}

TEST(SL_World_Alignment, alignment_ranges)
{
  Alignment a;
  a.set_alignment(-2500);

  EXPECT_EQ(ALIGNMENT_RANGE_EVIL, a.get_alignment_range());

  a.set_alignment(-1000);

  EXPECT_EQ(ALIGNMENT_RANGE_EVIL, a.get_alignment_range());

  a.set_alignment(-50);

  EXPECT_EQ(ALIGNMENT_RANGE_NEUTRAL, a.get_alignment_range());

  a.set_alignment(1166);

  EXPECT_EQ(ALIGNMENT_RANGE_GOOD, a.get_alignment_range());
}