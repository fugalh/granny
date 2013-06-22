#include <gtest/gtest.h>

#include "../Sndfile.hh"

TEST(Sndfile, ctor) {
  SF_INFO info;
  info.samplerate = 44100;
  info.channels = 2;
  info.format = SF_FORMAT_WAV;

  Sndfile s("/tmp/foo.wav", SFM_RDWR, &info);

  EXPECT_EQ(44100, s.info.samplerate);
  EXPECT_EQ(2, s.info.channels);
  EXPECT_EQ(SF_FORMAT_WAV, s.info.format);
}
