#include "OSCEngine.hh"

int main(int argc, char** argv)
{
  OSCEngine osc("1337", "wavs");
  osc.run();

  return 0;
}
