#include "OSC.hh"
#include "Sndbuf.hh"

#include <atomic>
#include <string>

class OSCEngine
{
public:
  OSCEngine(std::string port, std::string root);

  void run();

  std::atomic<bool> finished;

private:
  int event_cb(std::string path, OSC::Message);
  std::unordered_map<std::string, Sndbuf<float>> bufs_;
  OSC::Server srv_;
};
