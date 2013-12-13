# Be sure to get pyOSC from github or gitorious.
# The latest release tarball (3 years old) is buggy wrt time tags.
import OSC

import time

class Client(object):
    def __init__(self, addr):
        """ An address of the form (host, port) """
        self.client = OSC.OSCClient()
        self.client.connect(addr)

    def send(self, what, when=None):
        """
        Send an OSC message for the given event. If when is None, will use
        time.time()
        """

        if when is None:
            when = time.time()
        addr = '/event/' + what
        msg = OSC.OSCMessage(addr)
        bundle = OSC.OSCBundle(addr, when)
        bundle.append(msg)
        self.client.send(bundle)

    def pcap(self, bpf_filter, callback):
        """
        Live capture, will call back for every packet. Your callback should
        return a list of events to trigger as a result of this packet, e.g.
        ["get","set","get","delete"].  You will probably want to use dpkt to
        decode the packets.
        """

        # https://code.google.com/p/pypcap/
        import pcap

        try:
            p = pcap.pcap()
        except:
            print "Got root?"
            return

        p.setfilter(bpf_filter)

        for ts, pkt in p:
            for e in callback(ts, pkt):
                self.send(e)
