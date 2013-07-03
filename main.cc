#include "OSCEngine.hh"
#include "Jack.hh"
#include "Util.hh"

#include <thread>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  zmq::Context z;

  JackEngine je(&z, "jack");

  OSCEngine osc(OSC::Server("1337"), Util::glob("wavs/*.wav"), &z, "jack");
  osc.run();

  return 0;
}
