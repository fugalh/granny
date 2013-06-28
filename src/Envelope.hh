#pragma once

#include "Sndfile.hh"

struct Envelope {
  virtual ~Envelope() = default;
  virtual double at(sf_count_t pos) = 0;
};

struct OpenWindow : public Envelope {
  double at(sf_count_t pos) {
    return 1;
  }
};

