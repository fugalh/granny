#!/usr/bin/env python

# This example splits each packet by \r\n and makes an event out of the first
# word in each line. This will extract events out of memcache text protocol.

import granny

import re
import string
import time

printable = re.compile("^[%s]+$" % "".join(string.printable))

def payload(pkt):
    import dpkt
    eth = dpkt.ethernet.Ethernet(pkt)
    return eth.data.data.data

def first_word(ts, pkt):
    events = []
    for req in payload(pkt).split("\r\n"):
        words = req.split()
        if len(words) > 0:
            word0 = words[0]
            if re.match(printable, word0):
                events.append(word0)
    return events

# main
def parse_opts():
    import optparse
    op = optparse.OptionParser()
    op.add_option("-H", "--host", default="localhost")
    op.add_option("-P", "--port", default=1337)
    return op.parse_args()
(opts, args) = parse_opts()

gran = granny.Client((opts.host, opts.port))
gran.pcap("port 11302", first_word)
