#include <gtest/gtest.h>
#include <unistd.h>

#include "../Sndfile.hh"

using namespace std;
using testing::Test;

struct SndfileTest : public Test
{
  SndfileTest() : path("/tmp/foo.wav"), frames(100)
  {
    unlink(path.c_str());
    resetInfo();
  }

  void resetInfo()
  {
    info.samplerate = 44100;
    info.channels = 2;
    info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
  }

  void write()
  {
    resetInfo();
    Sndfile s(path, SFM_WRITE, &info);
    std::vector<float> v(frames * info.channels);
    s.write(v);
  }

  SF_INFO info;
  string path;
  sf_count_t frames;
};

TEST_F(SndfileTest, ctor)
{
  write();

  Sndfile s(path);

  EXPECT_EQ(info.samplerate, s.info.samplerate);
  EXPECT_EQ(info.channels, s.info.channels);
  EXPECT_EQ(info.format, s.info.format);
  EXPECT_EQ(frames, s.info.frames);
}

TEST_F(SndfileTest, read)
{
  write();

  Sndfile s(path, SFM_READ);
  auto v = s.read();

  EXPECT_EQ(info.samplerate, s.info.samplerate);
  EXPECT_EQ(info.channels, s.info.channels);
  EXPECT_EQ(info.format, s.info.format);
  EXPECT_EQ(frames, s.info.frames);
  EXPECT_EQ(frames * info.channels, v.size());
}
