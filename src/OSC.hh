#pragma once

#include <lo/lo.h>
#include <stdexcept>
#include <string>

namespace OSC {

struct Arg {
  Arg(char type, int32_t i)     : type_(type) { typeCheck('i'); arg_.i = i; }
  Arg(char type, float f)       : type_(type) { typeCheck('f'); arg_.f = f; }
  Arg(char type, std::string s) : type_(type) { typeCheck('s'); s_ = s; }
  Arg(char type, lo_timetag t)  : type_(type) { typeCheck('t'); t_ = t; }

  char type() { return type_; }

  int32_t     i() { typeCheck('i'); return arg_.i; }
  float       f() { typeCheck('f'); return arg_.f; }
  std::string s() { typeCheck('s'); return s_; }
  lo_timetag  t() { typeCheck('t'); return t_; }

protected:
  void typeCheck(char t) {
    if (type() != t)
      throw std::runtime_error("Wrong type");
  }


  char type_;

  lo_arg arg_;
  std::string s_;
  lo_timetag t_;
};

}
