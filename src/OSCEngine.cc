#include "OSCEngine.hh"

#include "Sndfile.hh"
#include "Util.hh"
#include "Grain.hh"

#include <memory>

using std::cout;
using std::endl;

using std::string;
using std::vector;

OSCEngine::OSCEngine(OSC::Server&& srv, vector<string> paths)
  : srv_(std::move(srv)), finished(false), dur_(10), env_(new OpenWindow)
{
  for (auto p : paths)
  {
    auto m = string("/event/") + Util::basename(p);

    bufs_[m] = std::make_shared<Sndbuf<float>>(Sndfile(p).read());

    srv_.addMethod(m, "",
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
  auto g = std::make_shared<Grain<>>(bufs_[path], dur_, env_);
  cout << path << endl;
  return 1;
}
