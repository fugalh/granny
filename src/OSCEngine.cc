#include "OSCEngine.hh"

#include "Sndfile.hh"
#include "Util.hh"

#include <iostream>
using std::cout;
using std::endl;

using std::string;

OSCEngine::OSCEngine(string port, string root) : srv_(port), finished(false)
{
  for (auto w : Util::glob(root + "/*.wav"))
  {
    auto p = string("/event/") + Util::basename(w, ".wav");

    bufs_[p] = Sndfile(w).read();

    srv_.addMethod(p, "",
                  // there's supposed to be a more direct way to do this :-P
                  [this](std::string path, OSC::Message msg) {
                    return this->event_cb(path, msg);
                  });
  }
}

void OSCEngine::run()
{
  while (!finished)
  {
    if (srv_.wait(10))
      srv_.recv();
  }
}

int OSCEngine::event_cb(string path, OSC::Message msg)
{
  cout << path << endl;
  return 1;
}
