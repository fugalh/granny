#include <gtest/gtest.h>
#include <unistd.h>

#include "Sndfile.hh"

using namespace std;
using testing::Test;

struct SndfileTest : public Test
{
  SndfileTest() : path("/tmp/foo.wav"), frames(8),
    buf(frames, 44100, 2, SF_FORMAT_WAV | SF_FORMAT_FLOAT)
  {
    unlink(path.c_str());
  }

  ~SndfileTest()
  {
    unlink(path.c_str());
  }

  void write()
  {
    SF_INFO info = buf.info();
    ASSERT_EQ(frames, buf.frames());
    Sndfile s(path, SFM_WRITE, &info);
    s.write(buf);
  }

  string path;
  sf_count_t frames;
  Sndbuf<float> buf;
};

TEST_F(SndfileTest, ctor)
{
  write();

  Sndfile s(path);

  SF_INFO info = buf.info();
  EXPECT_EQ(info.samplerate, s.info.samplerate);
  EXPECT_EQ(info.channels, s.info.channels);
  EXPECT_EQ(info.format, s.info.format);
  EXPECT_EQ(frames, s.info.frames);
}

TEST_F(SndfileTest, read_header)
{
  write();

  Sndfile s(path, SFM_READ);

  SF_INFO info = buf.info();
  EXPECT_EQ(info.samplerate, s.info.samplerate);
  EXPECT_EQ(info.channels, s.info.channels);
  EXPECT_EQ(info.format, s.info.format);
}

TEST_F(SndfileTest, write_then_read)
{
  write();

  auto b = Sndfile(path, SFM_READ).read();

  EXPECT_EQ(buf.samplerate, b.samplerate);
  EXPECT_EQ(buf.channels, b.channels);
  EXPECT_EQ(buf.format, b.format);

  EXPECT_EQ(buf.frames(), b.frames());
  EXPECT_EQ(buf.samples.size(), b.samples.size());
}
