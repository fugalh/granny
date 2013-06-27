#include "OSCEngine.hh"

#include "Util.hh"

#include <iostream>
using std::cout;
using std::endl;

using std::string;

void OSCEngine::run(string port)
{
  OSC::Server srv(port);

  auto wavs = Util::glob("wavs/*.wav");
  for (auto& w : wavs)
  {
    auto path = string("/event/") + Util::basename(w, ".wav");
    srv.addMethod(path, "",
                  // there's supposed to be a more direct way to do this :-P
                  [this](std::string path, OSC::Message msg) {
                    return this->event_cb(path, msg);
                  });
  }

  while (!finished)
  {
    if (srv.wait(10))
      srv.recv();
  }
}

int OSCEngine::event_cb(string path, OSC::Message msg)
{
  cout << path << endl;
  return 1;
}
