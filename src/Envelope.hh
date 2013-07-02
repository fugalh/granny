#pragma once

#include "Sndfile.hh"
#include "Util.hh"

#include <math.h>

struct Envelope
{
  virtual ~Envelope() = default;

  double* data() { return data_.data(); }

  void init(sf_count_t nsamples, double gain)
  {
    data_.resize(nsamples);

    calc(nsamples);

    auto d = data_.data();
    for (sf_count_t i = 0; i < nsamples; i++)
      d[i] *= gain;
  }

protected:
  virtual void calc(sf_count_t nsamples) = 0;

  std::vector<double> data_;
};

class OpenWindow : public Envelope {
public:
  OpenWindow(sf_count_t nsamples, double gain = 1)
  {
    init(nsamples, gain);
  }

private:
  void calc(sf_count_t nsamples)
  {
    auto d = data();
    for (sf_count_t i = 0; i < nsamples; i++)
      d[i] = 1;
  }
};

class HannWindow : public Envelope {
public:
  HannWindow(sf_count_t nsamples, double gain = 1)
  {
    init(nsamples, gain);
  }

private:
  void calc(sf_count_t nsamples)
  {
    auto d = data();
    for (sf_count_t i = 0; i < nsamples; i++)
      d[i] = (1.0 - cos(2.0 * M_PI * i / (nsamples - 1))) / 2.0;
  }
};
