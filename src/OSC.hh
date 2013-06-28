#pragma once

#include <functional>
#include <lo/lo.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>

namespace OSC {

class Arg {
public:
  Arg(char type, lo_arg* arg) : type_(type) {
    if (type == 's') {
      s_ = arg->s;
    } else {
      arg_ = *arg;
    }
  }

  Arg(char type, int32_t i)     : type_(type) { typeCheck('i'); arg_.i = i; }
  Arg(char type, float f)       : type_(type) { typeCheck('f'); arg_.f = f; }
  Arg(char type, std::string s) : type_(type), s_(s) { typeCheck('s'); }
  Arg(char type, lo_timetag t)  : type_(type) { typeCheck('t'); }

  char type() { return type_; }

  int32_t     i() { typeCheck('i'); return arg_.i; }
  float       f() { typeCheck('f'); return arg_.f; }
  std::string s() { typeCheck('s'); return s_; }
  lo_timetag  t() { typeCheck('t'); return arg_.t; }

  template <class T> T value();

private:
  void typeCheck(char t) {
    if (type() != t)
      throw std::runtime_error("Wrong type");
  }

  char type_;

  lo_arg arg_;
  std::string s_;
};

template <> inline int32_t     Arg::value<int32_t>()     { return i(); }
template <> inline float       Arg::value<float>()       { return f(); }
template <> inline std::string Arg::value<std::string>() { return s(); }
template <> inline lo_timetag  Arg::value<lo_timetag>()  { return t(); }

struct Message {
  Message(char const* types,
          lo_arg** argv,
          int argc,
          lo_message msg) : msg(msg) {
    for (int i = 0; i < argc; i++) {
      args.emplace_back(types[i], argv[i]);
    }
  }

  std::vector<Arg> args;
  lo_message msg;
};

struct Address {
  Address(std::string url) {
    addr = lo_address_new_from_url(url.c_str());
  }

  ~Address() {
    lo_address_free(addr);
  }

  lo_address addr;
};

class Server {
public:
  typedef std::function<int(std::string, Message)> Callback;

  Server() {
    server_ = lo_server_new(nullptr, nullptr);
  }

  Server(std::string port) {
    server_ = lo_server_new(port.c_str(), nullptr);
  }

  Server(Server&& other) : server_(nullptr) {
    std::swap(server_, other.server_);
    std::swap(methods_, other.methods_);
  }

  ~Server() {
    lo_server_free(server_);
  }

  int getPort() {
    return lo_server_get_port(server_);
  }

  Address getAddress() {
    return Address(lo_server_get_url(server_));
  }

  int wait(int timeout = 0) {
    return lo_server_wait(server_, timeout);
  }

  void addMethod(std::string path,
                 std::string typespec,
                 Callback func) {
    methods_[path] = func;
    lo_server_add_method(server_,
                         path.c_str(),
                         typespec.c_str(),
                         methodCallback,
                         (void*)this);
  }

  void recv() {
    lo_server_recv(server_);
  }

private:
  static int methodCallback(char const* path, char const* types,
                            lo_arg **argv, int argc, lo_message msg,
                            void* user_data) {
    Server* s = (Server*)user_data;
    if (s->methods_.find(path) == s->methods_.end())
      return 0;
    return s->methods_[path](std::string(path), Message(types, argv, argc, msg));
  }

  lo_server server_;
  std::unordered_map<std::string, Callback> methods_;
};

}
