#include <gtest/gtest.h>

#include "Sndbuf.hh"

using namespace std;
using testing::Test;

TEST(Sndbuf, frames)
{
  Sndbuf<> s(100, 44100, 2);

  EXPECT_EQ(s.frames(), 100);
}

TEST(Sndbuf, samples)
{
  Sndbuf<> s(100, 44100, 2);

  EXPECT_EQ(s.samples.size(), 200);
}
