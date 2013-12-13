#!/usr/bin/env python
import dpkt
import pcap
import OSC
import re
import string

p = pcap.pcap()
printable = re.compile("[%s]+" % "".join(string.printable))
p.setfilter('port 11302')

for ts, pkt in p:
    eth = dpkt.ethernet.Ethernet(pkt)
    payload = eth.data.data.data
    words = payload.strip().split()
    if len(words) > 0:
        word = words[0]
        if re.match(printable, word):
            print "%s: %s" % (ts, word)
