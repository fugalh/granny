#include <gtest/gtest.h>

#include "../Envelope.hh"

TEST(Hann, zero_at_edges)
{
  HannWindow h(10);

  EXPECT_EQ(0, h.data()[0]);
  EXPECT_EQ(0, h.data()[9]);
}

TEST(Hann, big_in_middle)
{
  HannWindow h(10);

  EXPECT_GT(h.data()[5], 0.9);
}
