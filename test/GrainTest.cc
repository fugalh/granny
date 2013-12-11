#include <gtest/gtest.h>
#include <vector>
#include <memory>

#define private public
#include "Grain.hh"

using namespace std;

struct GrainTest : public ::testing::Test {
  GrainTest()
    : frames_(100), buf_(new Sndbuf<float>(frames_)), win_(new OpenWindow(frames_)) {}

  sf_count_t frames_;
  shared_ptr<Sndbuf<float>> buf_;
  shared_ptr<Envelope> win_;
};

TEST_F(GrainTest, random_start_is_random) {
  // P(100 grains all start at 0) is ludicrously small
  bool all_are_zero = true;
  for (int i = 0; i < 100; i++)
  {
    Grain<float> g(buf_, frames_ / 10, win_);
    all_are_zero &= (g.start_ == 0);
  }

  EXPECT_FALSE(all_are_zero);
}

TEST_F(GrainTest, full_length_is_0) {
  Grain<float> g(buf_, frames_, win_);
  EXPECT_EQ(0, g.start_);
}

TEST_F(GrainTest, too_long_is_0) {
  Grain<float> g(buf_, frames_ + 1, win_);
  EXPECT_EQ(0, g.start_);
}

TEST_F(GrainTest, start_has_room) {
  auto len = 10;
  Grain<float> g(buf_, len, win_);
  EXPECT_LE(len, g.buf_->frames() - g.start_);
}

TEST_F(GrainTest, start_has_room_when_full) {
  Grain<float> g(buf_, frames_, win_);
  EXPECT_EQ(frames_, g.buf_->frames() - g.start_);
}
