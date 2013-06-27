#include "util.hh"

#include <glob.h>
#include <stdexcept>
#include <iostream>

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

  globfree(&g);
  return paths;
}


string basename(string path, string suffix)
{
  auto a = path.rfind("/");
  if (a == string::npos)
    a = 0;
  else
    a++; // omit the slash

  path = path.substr(a);

  auto b = path.rfind(suffix);
  if (b != string::npos)
    path = path.substr(0, b);

  return path;
}

}
