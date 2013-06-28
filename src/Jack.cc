#include "Grain.hh"
#include "Jack.hh"
#include "Util.hh"

#include <memory>
#include <iostream>
#include <stdexcept>

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
  auto buf = get_buffer(nframes);

  while (true)
  {
    unique_ptr<Grain<float>> grain(zmq_.recv<Grain<float>>());
    if (!grain)
      break;

    auto offset = grain_offset(grain.get());

    log("grain", grain->time, jack_last_frame_time(client_), offset);
  }

  return 0;
}

JackEngine::sample_t* JackEngine::get_buffer(jack_nframes_t nframes)
{
  return (sample_t*)jack_port_get_buffer(port_, nframes);
}

jack_time_t JackEngine::grain_offset(Grain<float> const* grain)
{
  jack_time_t latency = 10 * 1000 * 1000; // 1 second
  jack_nframes_t f = jack_time_to_frames(client_, grain->time + latency);
  return std::max<jack_nframes_t>(0, f - jack_last_frame_time(client_));
}
