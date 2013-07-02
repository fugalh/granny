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
    : buf_(buf), env_(env), len(len), start_(random_start()), time(jack_get_time())
  {}

  S sample_at(sf_count_t pos, int channel = 0)
  {
    auto i = (pos + start_) * buf_->channels + channel;
    return env_->data()[pos] * buf_->samples[i];
  }

  jack_time_t time;
  sf_count_t len;

private:
  sf_count_t random_start()
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
