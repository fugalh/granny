#include "OSCEngine.hh"

#include "Sndfile.hh"
#include "Util.hh"
#include "Grain.hh"

#include <memory>

using std::cout;
using std::endl;

using std::string;
using std::vector;

OSCEngine::OSCEngine(OSC::Server&& srv, vector<string> paths,
                     zmq::Context* zctx, string zendpoint)
  : srv_(std::move(srv)), finished(false), dur_(4410), env_(new HannWindow(dur_)),
    zmq_(zctx, zendpoint)
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

    srv_.addMethod(m, "i",
      // there's supposed to be a more direct way to do this :-P
      [this](std::string path, OSC::Message msg) {
        if (msg.args[0].i() == 1)
          return this->event_cb(path, msg);
        return 0;
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
  auto g = new Grain<float>(bufs_[path], dur_, env_);
  g->time += 10000; // add some latency to account for liblo delay
  //Util::log(path, g->time);
  zmq_.send(g);
  return 1;
}
