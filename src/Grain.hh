#pragma once

#include "Sndbuf.hh"
#include <iostream>
#include <random>

struct Window {
  virtual ~Window() = default;
  virtual double at(sf_count_t pos) = 0;
};

template <class S = float>
struct Grain {
  Grain(std::shared_ptr<Sndbuf<S>> buf,
        sf_count_t len,
        std::shared_ptr<Window> win);

  S sample_at(sf_count_t pos, int channel = 0);

private:
  sf_count_t random_start(sf_count_t len);

  std::shared_ptr<Sndbuf<S>> buf_;
  sf_count_t start_;
  std::shared_ptr<Window> win_;
};

template <class S>
Grain<S>::Grain(std::shared_ptr<Sndbuf<S>> buf, sf_count_t len, std::shared_ptr<Window> win)
  : buf_(buf), win_(win), start_(random_start(len)) {}

template <class S>
sf_count_t Grain<S>::random_start(sf_count_t len)
{
  int n = buf_->frames() - len;

  if (n <= 0)
    return 0;

  return std::rand() % n;
}

template <class S>
S Grain<S>::sample_at(sf_count_t pos, int channel)
{
  return (S)(win_->at(pos) * buf_.samples[pos + start_ + channel]);
}

