#pragma once

#include <sndfile.h>
#include <stdexcept>

template <class S = float>
class Sndbuf {
public:
  Sndbuf(SF_INFO const& info);
  Sndbuf(sf_count_t frames = 0,
         int samplerate = 44100,
         int channels = 2,
         int format = SF_FORMAT_WAV | SF_FORMAT_FLOAT);

  sf_count_t frames() const;
  SF_INFO info();

  void resizef(sf_count_t frames)
  {
    samples.resize(frames * channels);
  }

  int samplerate;
  int channels;
  int format;
  std::vector<S> samples;
};

template <class S>
Sndbuf<S>::Sndbuf(SF_INFO const& info) :
  samplerate(info.samplerate),
  channels(info.channels),
  format(info.format),
  samples(info.frames * channels)
{}

template <class S>
Sndbuf<S>::Sndbuf(sf_count_t frames,
               int samplerate,
               int channels,
               int format) :
  samplerate(samplerate), channels(channels), format(format), samples(frames * channels)
{}

template <class S>
sf_count_t Sndbuf<S>::frames() const
{
  auto n = samples.size();
  if (n % channels != 0)
    throw std::runtime_error("Number of samples must be an integral product of number of channels.");
  return n / channels;
}

template <class S>
SF_INFO Sndbuf<S>::info()
{
  SF_INFO info;

  info.frames = frames();
  info.samplerate = samplerate;
  info.channels = channels;
  info.format = format;

  return info;
}
