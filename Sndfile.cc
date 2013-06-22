#include "Sndfile.hh"

#include <string>
#include <vector>

using std::string;
using std::vector;

Sndfile::Sndfile(string const& path, int mode, SF_INFO* info)
{
  if (info == nullptr)
    this->info.format = 0;
  else
    this->info = *info;

  sf_ = sf_open(path.c_str(), mode, &this->info);
}
