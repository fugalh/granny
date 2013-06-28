#include "zmq.hh"

#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::string;
using std::to_string;

namespace zmq {
  void throw_perror(string msg) {
    throw std::runtime_error(msg + ": " + zmq_strerror(zmq_errno()));
  }

  Context::Context()
  {
    ctx_ = zmq_ctx_new();
  }

  Context::~Context()
  {
    zmq_ctx_destroy(ctx_);
  }

  Socket::Socket(Context* ctx, int type) : ctx_(ctx)
  {
    sock_ = zmq_socket(ctx_->ctx_, type);
    if (sock_ == nullptr)
      perror("zmq_socket");
  }

  Socket::~Socket()
  {
    zmq_close(sock_);
  }

  Source::Source(Context* ctx, string name) : Socket(ctx, ZMQ_PAIR)
  {
    auto endpoint = string("inproc://") + name;
    if (0 != zmq_connect(sock_, endpoint.c_str()))
      throw_perror(string("zmq_connect to ") + endpoint);
  }

  Sink::Sink(Context* ctx, string name) : Socket(ctx, ZMQ_PAIR)
  {
    auto endpoint = string("inproc://") + name;
    if (0 != zmq_bind(sock_, endpoint.c_str()))
      throw_perror(string("zmq_bind to ") + endpoint);
  }
}
