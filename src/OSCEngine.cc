#include "OSCEngine.hh"

using std::string;

void OSCEngine::run(string port)
{
  OSC::Server srv(port);

  srv.addMethod("/event/foo", "",
                // there's supposed to be a more direct way to do this :-P
                [this](std::string path, OSC::Message msg) {
                  return this->event_cb(path, msg);
                });

  while (!finished)
  {
    if (srv.wait(10))
      srv.recv();
  }
}

#include <iostream>
using std::cout;
using std::endl;

int OSCEngine::event_cb(string path, OSC::Message msg)
{
  cout << path << endl;
  return 1;
}
