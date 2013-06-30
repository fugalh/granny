#pragma once

#include "Sndfile.hh"
#include "Util.hh"

#include <math.h>

struct Envelope {
  virtual ~Envelope() = default;
  virtual double at(sf_count_t pos) = 0;
};

struct OpenWindow : public Envelope {
  double at(sf_count_t pos) {
    return 1;
  }
};

class HannWindow : public Envelope {
public:
  HannWindow(sf_count_t dur) : dur_(dur) {}

  double at(sf_count_t pos) {
    auto hann = (1.0 - cos(2.0 * M_PI * pos / (dur_ - 1))) / 2.0;
    return hann;
  }

private:
  sf_count_t dur_;
};

