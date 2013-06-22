#pragma once

#include <sndfile.h>
#include <stdio.h>
#include <string>
#include <vector>

/** Basic read/write functionality. No seeking etc. */
class Sndfile {
public:
  Sndfile(std::string const& path, int mode = SFM_READ, SF_INFO* info = nullptr);

  std::vector<float> read();
  void write(std::vector<float> const&);

  SF_INFO info;

private:
  SNDFILE* sf_;
};
