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

vector<float> Sndfile::read()
{
  sf_count_t items = info.frames * info.channels;

  vector<float> v(items);

  sf_seek(sf_, 0, SEEK_SET);
  sf_count_t xfer = sf_read_float(sf_, v.data(), items);
  v.resize(xfer); // in case we reach premature EOF

  return v;
}

void Sndfile::write(vector<float> const& v)
{
  if (v.size() % info.channels != 0)
    throw std::runtime_error("Number of samples must be an integer product of the number of channels.");

  sf_count_t xfer = sf_write_float(sf_, v.data(), v.size());

  if (xfer != v.size())
    throw std::runtime_error("Didn't write all items.");
}
