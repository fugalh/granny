#pragma once

#include <sndfile.h>
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "Sndbuf.hh"

/** Basic read/write functionality. No seeking etc. */
class Sndfile {
public:
  Sndfile(std::string const& path, int mode = SFM_READ, SF_INFO* info = nullptr);
  ~Sndfile();

  template <class S = float>
  Sndbuf<S> read();

  template <class S>
  void write(Sndbuf<S> const&);

  SF_INFO info;

private:
  SNDFILE* sf_;
};

namespace {

template <class S>
static sf_count_t sf_writef(SNDFILE*, S*, sf_count_t);

template <class S>
static sf_count_t sf_readf(SNDFILE*, S*, sf_count_t);

template<>
sf_count_t sf_writef<float const>(SNDFILE *sf, float const *ptr, sf_count_t frames)
{
  return sf_writef_float(sf, ptr, frames);
}

template<>
sf_count_t sf_readf<float>(SNDFILE *sf, float *ptr, sf_count_t frames)
{
  return sf_readf_float(sf, ptr, frames);
}

} // anonymous namespace

template <class S>
Sndbuf<S> Sndfile::read()
{
  Sndbuf<S> buf(info);

  sf_seek(sf_, 0, SEEK_SET);
  sf_count_t xfer = sf_readf(sf_, buf.samples.data(), info.frames);
  buf.resizef(xfer); // in case we reach premature EOF

  return buf;
}

template <class S>
void Sndfile::write(Sndbuf<S> const& buf)
{
  sf_count_t xfer = sf_writef(sf_, buf.samples.data(), buf.frames());

  if (xfer != buf.frames())
    throw std::runtime_error("Didn't write all frames.");
}
