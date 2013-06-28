#pragma once

#include "Sndbuf.hh"
#include "Envelope.hh"

#include <iostream>
#include <jack/jack.h>
#include <memory>
#include <random>
#include <vector>

template <class S = float>
struct Grain {
  Grain(std::shared_ptr<Sndbuf<S>> buf,
        sf_count_t len,
        std::shared_ptr<Envelope> env)
    : buf_(buf), env_(env), start_(random_start(len)), time(jack_get_time())
  {}

  S sample_at(sf_count_t pos, int channel = 0)
  {
    return (S)(env_->at(pos) * buf_.samples[pos + start_ + channel]);
  }

  jack_time_t time;

private:
  sf_count_t random_start(sf_count_t len)
  {
    int n = buf_->frames() - len;

    if (n <= 0)
      return 0;

    return std::rand() % n;
  }

  std::shared_ptr<Sndbuf<S>> buf_;
  sf_count_t start_;
  std::shared_ptr<Envelope> env_;
};
