#include "Grain.hh"
#include "OSC.hh"
#include "Sndbuf.hh"
#include "zmq.hh"

#include <atomic>
#include <memory>
#include <string>
#include <vector>

class OSCEngine
{
public:
  OSCEngine(OSC::Server&&, std::vector<std::string> paths,
            zmq::Context*, std::string zmq_endpoint);

  void run();

  std::atomic<bool> finished;

private:
  int event_cb(std::string path, OSC::Message);
  std::unordered_map<std::string, std::shared_ptr<Sndbuf<float>>> bufs_;
  OSC::Server srv_;
  sf_count_t dur_;
  std::shared_ptr<Envelope> env_;
  zmq::Source zmq_;
};
