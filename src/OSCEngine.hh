#include "OSC.hh"

#include <atomic>
#include <string>

class OSCEngine
{
public:
  void run(std::string port);

  std::atomic<bool> finished;

private:
  void event_cb(std::string path, OSC::Message);
};
