#include "Jack.hh"

#include <stdexcept>
#include <iostream>

JackEngine::JackEngine()
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
    free (ports);
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

  // success
  return 0;
}

