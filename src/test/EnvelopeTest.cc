#include <gtest/gtest.h>

#include "../Envelope.hh"

TEST(Hann, zero_at_edges)
{
  HannWindow h(10);

  EXPECT_EQ(0, h.at(0));
  EXPECT_EQ(0, h.at(9));
}

TEST(Hann, big_in_middle)
{
  HannWindow h(10);

  EXPECT_GT(h.at(5), 0.9);
}
