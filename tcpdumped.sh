# read sandbox and pretend all packets are gets. tcpdump doesn't give date, so we have to add one
tcpdump -r mc.sandbox.pcap | awk '{print "get 2013/06/28", $1}' | python -u ./forward.py localhost
