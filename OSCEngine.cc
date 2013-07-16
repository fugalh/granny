#include "OSCEngine.hh"

#include "Sndfile.hh"
#include "Util.hh"
#include "Grain.hh"
#include "Options.hh"

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
  int i = 1;
  for (auto p : paths)
  {
    auto m = string("/event/") + Util::basename(p);

    bufs_[m] = std::make_shared<Sndbuf<float>>(Sndfile(p).read());

    srv_.addMethod(m, "",
      // there's supposed to be a more direct way to do this :-P
      [this](std::string path, OSC::Message msg) {
        return this->event_cb(path, msg);
      });

    // Support a column of TouchOSC multi-push buttons
    srv_.addMethod(string("/event/1/") + std::to_string(i++), "i",
      [=](std::string path, OSC::Message msg) {
        if (msg.args[0].i() == 1)
          return this->event_cb(m, msg);
        return 0;
      });
  }

  srv_.addMethod("/control/duration", "f",
    [this](std::string path, OSC::Message msg) {
      auto ms = msg.args[0].f();
      auto s = ms / 1000;
      auto sr = 44100; // TODO get this from jack
      dur_ = sr * s;
      return 1;
    });

  srv_.addMethod("/control/gain", "f",
    [this](std::string path, OSC::Message msg) {
      env_.reset(new HannWindow(dur_, msg.args[0].f()));
      return 1;
    });
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
  if (bufs_.find(path) == bufs_.end())
    return 0;

  if (verbose)
    Util::log(path);

  // XXX If the grain is not as long as dur_ things will get ugly. Better to
  // recalculate the envelope for each duration (and cache)
  auto g = new Grain<float>(bufs_[path], dur_, env_);
  g->time += 1e3; // add some latency to account for pipeline delay
  zmq_.send(g);
  return 1;
}
