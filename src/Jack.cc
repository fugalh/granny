#include "Grain.hh"
#include "Jack.hh"
#include "Util.hh"

#include <ctime>
#include <memory>
#include <iostream>
#include <jack/statistics.h>
#include <stdexcept>
#include <string.h>

using std::cout;
using std::endl;
using Util::log;
using std::unique_ptr;

JackEngine::JackEngine(zmq::Context* zctx, std::string zendpoint)
  : zmq_(zctx, zendpoint)
{
  client_ = jack_client_open("granny", JackNullOption, nullptr);
  if (client_ == nullptr)
    throw std::runtime_error("Couldn't open jack client.");

  if (0 != jack_set_process_callback(client_, process_callback, this))
    throw std::runtime_error("Couldn't set process callback.");

  port_ = jack_port_register(client_, "output", JACK_DEFAULT_AUDIO_TYPE,
                             JackPortIsOutput|JackPortIsTerminal, 0);
  if (port_ == nullptr)
    throw std::runtime_error("Couldn't crate jack port.");

  jack_set_xrun_callback(client_, on_xrun, this);

  if (0 != jack_activate(client_))
    throw std::runtime_error("Couldn't activate jack.");

  const char **ports = jack_get_ports(client_, nullptr, nullptr,
                                      JackPortIsPhysical|JackPortIsInput);
  if (ports)
  {
    jack_connect(client_, jack_port_name(port_), ports[0]);
    jack_free(ports);
  }
}

JackEngine::~JackEngine()
{
  jack_port_unregister(client_, port_);
  jack_client_close(client_);
}

int JackEngine::process_callback(jack_nframes_t nframes, void* arg)
{
  JackEngine* je = (JackEngine*)arg;

  return je->process(nframes);
}


int JackEngine::process(jack_nframes_t nframes)
{
  while (true)
  {
    auto grain = zmq_.recv_nonblocking<Grain<float>>();
    if (!grain)
      break;

    grains_in_process_.emplace_back(grain);
  }

  process_grains(nframes);

  return 0;
}

JackEngine::sample_t* JackEngine::get_buffer(jack_nframes_t nframes)
{
  return (sample_t*)jack_port_get_buffer(port_, nframes);
}

ssize_t JackEngine::grain_offset(Grain<float> const* grain)
{
  ssize_t f = jack_time_to_frames(client_, grain->time);
  jack_nframes_t now = jack_last_frame_time(client_);

  return f - now;
}

void JackEngine::process_grains(jack_nframes_t nframes)
{
  auto buf = get_buffer(nframes);
  memset(buf, 0, sizeof(sample_t) * nframes);

  for (auto& grain : grains_in_process_)
  {
    ssize_t ji = grain_offset(grain.get());
    ssize_t gi = 0;

    if (ji < 0)
    {
      gi += -ji;
      ji = 0;
    }

    while (gi < grain->len && ji < nframes)
    {
      buf[ji] += grain->sample_at(gi);
      gi++;
      ji++;
    }
  }

  for (auto i = 0; i < nframes; i++)
    buf[i] *= 0.05;

  grains_in_process_.remove_if(
    [&](unique_ptr<Grain<float>> const& g) {
      return -grain_offset(g.get()) > g->len;
    });
}

int JackEngine::on_xrun(void* arg)
{
  JackEngine* je = (JackEngine*)arg;
  std::time_t t = std::time(nullptr);
  Util::log("xrun", jack_get_xrun_delayed_usecs(je->client_) / 1e6,
            "at", std::ctime(&t));
  return 0;
}
