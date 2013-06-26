#include <gtest/gtest.h>

#include "../OSC.hh"

using namespace OSC;
using namespace std;
using testing::Test;

TEST(Arg, ctor) {
#define CHAR(x) 'x'
#define TEST_ARG(T, v) {\
  Arg a(CHAR(X), (v));\
  EXPECT_EQ(CHAR(X), a.type());\
  EXPECT_EQ((v), a.T());\
}

  TEST_ARG(i, 42);
  TEST_ARG(f, 3.1419f);
  TEST_ARG(s, "foo");

#undef CHAR
#undef TEST_ARG
}

TEST(Arg, badTypeThrows) {
  Arg a('i', 42);

  EXPECT_THROW(a.f(), std::runtime_error);
}

TEST(Arg, timeTag) {
  lo_timetag tt { 3, 14149 };
  Arg a('t', tt);
}
