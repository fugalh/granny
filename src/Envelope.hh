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
  HannWindow(sf_count_t dur, double gain = 1) : dur_(dur), gain_(gain) {}

  double at(sf_count_t pos) {
    auto hann = (1.0 - cos(2.0 * M_PI * pos / (dur_ - 1))) / 2.0;
    return hann * gain_;
  }

private:
  sf_count_t dur_;
  double gain_;
};

