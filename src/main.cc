#include "Options.hh"
#include "OSCEngine.hh"
#include "Jack.hh"
#include "Util.hh"

#include <getopt.h>
#include <thread>
#include <iostream>

using namespace std;

void usage(char* name) {
  cout << "usage: " << name << " [-v]" << endl; 
  exit(1);
}

void parse_options(int argc, char** argv) {
  int ch;
  while ((ch = getopt(argc, argv, "hv")) != -1) {
    switch (ch) {
    case 'v':
      verbose = true;
      break;
    case 'h':
    default:
      usage(argv[0]);
    }
  }
}

int main(int argc, char** argv)
{
  parse_options(argc, argv);

  zmq::Context z;

  JackEngine je(&z, "jack");

  OSCEngine osc(OSC::Server("1337"), Util::glob("wavs/*.wav"), &z, "jack");
  osc.run();

  return 0;
}
