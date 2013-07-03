#include <gtest/gtest.h>
#include <thread>

#include "../OSC.hh"

using namespace OSC;
using namespace std;
using testing::Test;
using std::vector;
using std::string;
using std::thread;

template <class T>
static void testArgTypeAndValue(char t, T const& v) {
  Arg a(t, v);
  EXPECT_EQ(t, a.type());
  EXPECT_EQ(v, a.value<T>());
}

TEST(Arg, typeAndValue) {
  testArgTypeAndValue('i', 42);
  testArgTypeAndValue('f', 3.14149f);
  testArgTypeAndValue('s', string("foo"));
}

TEST(Arg, badTypeThrows) {
  Arg a('i', 42);

  EXPECT_THROW(a.f(), std::runtime_error);
}

TEST(Arg, timeTag) {
  lo_timetag tt { 3, 14149 };
  Arg a('t', tt);
}

TEST(Server, ctor) {
  Server();
  Server("12345");
}

TEST(Server, getPort) {
  EXPECT_LT(1024, Server().getPort());
}

TEST(Server, wait) {
  Server s;
  bool flag = false;

  thread t([&] {
    s.wait(1000);
    flag = true;
  });

  EXPECT_FALSE(flag);

  lo_send(s.getAddress().addr, "/foo", "if", 42, 3.14159);
  t.join();
}

TEST(Server, addMethod) {
  Server s;
  bool flag = false;

  s.addMethod("/event/any", "if", [&](string, Message) { flag = true; return 1; });
  lo_send(s.getAddress().addr, "/event/*", "if", 42, 3.14159);
  s.recv();

  EXPECT_TRUE(flag);
}
