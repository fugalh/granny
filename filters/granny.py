# Be sure to get pyOSC from github or gitorious.
# The latest release tarball (3 years old) is buggy wrt time tags.
import OSC

import time

class Client(object):
    def __init__(self, addr):
        self.client = OSC.OSCClient()
        self.client.connect(addr)

    def send(self, what, when=None):
        if when is None:
            when = time.time()
        addr = '/event/' + what
        msg = OSC.OSCMessage(addr)
        bundle = OSC.OSCBundle(addr, when)
        bundle.append(msg)
        self.client.send(bundle)
