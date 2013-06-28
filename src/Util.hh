#pragma once

#include <glob.h>
#include <iostream>
#include <string>
#include <vector>

namespace Util {

  /** Glob like glob(3) from glob.h */
  std::vector<std::string> glob(std::string pattern,
                                int flags = GLOB_NOSORT|GLOB_BRACE|GLOB_TILDE);

  std::string basename(std::string path, bool strip_suffix = true);

  void log();

  template <class T, class ...Args>
  void log(T arg0, Args... args)
  {
    std::cout << arg0 << ' ';
    log(args...);
  }
}
