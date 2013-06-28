#pragma once

#include "Sndfile.hh"
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
    return (1 + cos(2 * M_PI / (dur_ - 1))) / 2;
  }

private:
  sf_count_t dur_;
};

