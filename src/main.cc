#include "OSCEngine.hh"
#include "Jack.hh"
#include "Util.hh"

#include <thread>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  thread osc_thread([] {
    OSCEngine osc(OSC::Server("1337"), Util::glob("wavs/*.wav"));
    osc.run();
  });

  JackEngine je;

  osc_thread.join();

  return 0;
}
