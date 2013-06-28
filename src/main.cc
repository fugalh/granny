#include "OSCEngine.hh"
#include "Util.hh"

int main(int argc, char** argv)
{
  OSCEngine osc(OSC::Server("1337"), Util::glob("wavs/*.wav"));
  osc.run();

  return 0;
}
