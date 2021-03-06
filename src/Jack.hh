#pragma once

#include "zmq.hh"

#include <jack/jack.h>
#include <list>

class JackEngine {
public:
  typedef jack_default_audio_sample_t sample_t;

  JackEngine(zmq::Context*, std::string zmq_endpoint);
  ~JackEngine();

private:
  static int process_callback(jack_nframes_t, void*);
  static int on_xrun(void*);

  int process(jack_nframes_t);
  sample_t* get_buffer(jack_nframes_t);

  /* Actual grain time + nominal latency, offset from block time */
  ssize_t grain_offset(Grain<float> const* grain);

  void process_grains(jack_nframes_t);

  jack_client_t* client_;
  jack_port_t* port_;
  zmq::Sink zmq_;

  std::list<std::unique_ptr<Grain<float>>> grains_in_process_;
};
