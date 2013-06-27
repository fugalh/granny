#pragma once

#include <string>
#include <vector>

namespace Util {

  /** Glob like glob(3) from glob.h */
  std::vector<std::string> glob(std::string pattern, int flags);

}
