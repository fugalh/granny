#pragma once

#include <glob.h>
#include <string>
#include <vector>

namespace Util {

  /** Glob like glob(3) from glob.h */
  std::vector<std::string> glob(std::string pattern,
                                int flags = GLOB_NOSORT|GLOB_BRACE|GLOB_TILDE);

  std::string basename(std::string path, std::string suffix = "");

}
