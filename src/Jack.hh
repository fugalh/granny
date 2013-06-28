#pragma once

#include <jack/jack.h>

class JackEngine {
public:
  JackEngine();
  ~JackEngine();

private:
  static int process_callback(jack_nframes_t, void*);

  jack_client_t* client_;
  jack_port_t* port_;
};
