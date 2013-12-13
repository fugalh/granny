#!/usr/bin/env python
import granny

import dpkt
import pcap

import re
import string
import time

printable = re.compile("^[%s]+$" % "".join(string.printable))

try:
    p = pcap.pcap()
except:
    print "Got root?"
    exit(1)

p.setfilter('port 11302')

def parse_opts():
    import optparse
    op = optparse.OptionParser()
    op.add_option("-H", "--host", default="localhost")
    op.add_option("-P", "--port", default=1337)
    return op.parse_args()
(opts, args) = parse_opts()

gran = granny.Client((opts.host, opts.port))

def payload(pkt):
    eth = dpkt.ethernet.Ethernet(pkt)
    return eth.data.data.data

for ts, pkt in p:
    words = payload(pkt).rstrip().split()
    if len(words) > 0:
        word = words[0]
        if re.match(printable, word):
            print word
            gran.send(word)
