#include "Sndfile.hh"

#include <string>
#include <vector>
#include <stdexcept>

using std::string;
using std::vector;



Sndfile::Sndfile(string const& path, int mode, SF_INFO* info)
{
  if (info)
    this->info = *info;
  else
    this->info.format = 0;

  sf_ = sf_open(path.c_str(), mode, &this->info);

  if (info)
    *info = this->info;

  if (sf_ == nullptr)
    throw std::runtime_error(sf_strerror(sf_));
}

Sndfile::~Sndfile()
{
  if (sf_)
    sf_close(sf_);
}
