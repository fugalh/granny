#pragma once

#include <lo/lo.h>
#include <stdexcept>
#include <string>

namespace OSC {

class Arg {
public:
  Arg(char type, int32_t i)     : type_(type) { typeCheck('i'); arg_.i = i; }
  Arg(char type, float f)       : type_(type) { typeCheck('f'); arg_.f = f; }
  Arg(char type, std::string s) : type_(type), s_(s) { typeCheck('s'); }
  Arg(char type, lo_timetag t)  : type_(type), t_(t) { typeCheck('t'); }

  char type() { return type_; }

  int32_t     i() { typeCheck('i'); return arg_.i; }
  float       f() { typeCheck('f'); return arg_.f; }
  std::string s() { typeCheck('s'); return s_; }
  lo_timetag  t() { typeCheck('t'); return t_; }

  template <class T> T value();

private:
  void typeCheck(char t) {
    if (type() != t)
      throw std::runtime_error("Wrong type");
  }

  char type_;

  lo_arg arg_;
  std::string s_;
  lo_timetag t_;
};

template <> int32_t     Arg::value<int32_t>()     { return i(); }
template <> float       Arg::value<float>()       { return f(); }
template <> std::string Arg::value<std::string>() { return s(); }
template <> lo_timetag  Arg::value<lo_timetag>()  { return t(); }

class Server {
public:
  Server(std::string port = "") {
    char const* p = nullptr;
    if (port != "")
      p = port.c_str();
    server_ = lo_server_new(p, nullptr);
  }

  ~Server() {
    lo_server_free(server_);
  }

  int getPort() {
    return lo_server_get_port(server_);
  }

private:
  lo_server server_;
};

}
