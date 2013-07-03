#!python -u

# Be sure to get pyOSC from github or gitorious.
# The latest release tarball (3 years old) is buggy wrt time tags.
import OSC
import sys
import time
import traceback

class Far(object):
    def __init__(self, addr):
        self.client = OSC.OSCClient()
        self.client.connect(addr)

    def send(self, what, when):
        addr = '/event/' + what
        msg = OSC.OSCMessage(addr)
        bundle = OSC.OSCBundle(addr, when)
        bundle.append(msg)
        self.client.send(bundle)

far = Far((sys.argv[1], 1337))
first = None
now = time.time()

for line in sys.stdin:
    try:
        line = line.strip().split()
        if len(line) != 1 and len(line) != 3:
            continue

        what = line[0]
        when = None
        if len(line) > 1:
            when = " ".join(line[1:])
            when, micros = (when + ".").split('.', 1)
            when = time.mktime(time.strptime(when, "%Y/%m/%d %H:%M:%S"))
            when += float(micros) / 1e6

            if when < now:
                if first is None:
                    first = now - when
                    print first

                when += first

        if when is None:
            when = time.time()

        # buffer in some latency
        when += 1

        far.send(what, when)
    except:
        traceback.print_exc()
