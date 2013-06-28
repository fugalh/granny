#pragma once

/* Apparently the C++ bindings aren't installed by "port install zmq", and I
   want to keep this simple for the user and a little C API doesn't scare me, so... */
#include <zmq.h>
#include <string>

#include "Util.hh"

using Util::log;

namespace zmq {

class Context {
public:
  Context();
  ~Context();

protected:
  friend class Socket;
  void* ctx_;
};

class Socket {
public:
  Socket(Context*, int type);
  virtual ~Socket();

protected:
  Context* ctx_;
  void* sock_;
};

class Source : public Socket {
public:
  Source(Context*, std::string name);
  template <class T> int send(T* ptr, int flags = 0)
  {
    int ret;

    do {
      ret = zmq_send(sock_, &ptr, sizeof(ptr), flags);
    } while (ret == -1 && (errno == EAGAIN || errno == EINTR));

    return ret;
  }
};

class Sink : public Socket {
public:
  Sink(Context*, std::string name);

  template <class T> T* recv(bool blocking = true)
  {
    T* ptr;
    int ret;

    do {
      ret = zmq_recv(sock_, &ptr, sizeof(ptr),
                     blocking ? 0 : ZMQ_DONTWAIT);
    } while (ret == -1 &&
             (errno == EINTR || (blocking && errno == EAGAIN)));

    if (ret == -1)
      ptr = nullptr;

    return ptr;
  }

  template <class T> T* recv_nonblocking()
  {
    return recv<T>(false);
  }

};

}

