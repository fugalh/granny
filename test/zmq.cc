#include "../zmq.hh"
#include <gtest/gtest.h>

#include <string>
#include <thread>

using std::string;
using std::thread;

struct ZmqTest : public ::testing::Test {
  ZmqTest() : name("test"), jack(&ctx, name), osc(&ctx, name) {}

  zmq::Context ctx;
  string name;
  zmq::Sink jack;
  zmq::Source osc;
};

TEST_F(ZmqTest, txrx) {
  int x = 42;
  int y = 0;

  thread t([&] {
    y = *jack.recv<int>(false);
  });
  osc.send<int>(&x);

  t.join();

  EXPECT_EQ(x, y);
  EXPECT_EQ(42, y);
}
