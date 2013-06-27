#include "util.hh"

#include <glob.h>
#include <stdexcept>

using namespace std;

namespace Util {

vector<string> glob(string pattern, int flags)
{
  glob_t g;
  if (0 != ::glob(pattern.c_str(), flags, nullptr, &g))
    throw std::runtime_error("glob failed");

  vector<string> paths(g.gl_matchc);
  for (int i = 0; i < g.gl_matchc; i++)
    paths[i] = g.gl_pathv[i];

  return paths;
}

}
