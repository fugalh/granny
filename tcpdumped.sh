# read sandbox and pretend all packets are gets. tcpdump doesn't give date, so we have to add one
event=${1:-get}
date=`date +%Y-%m-%d -d 'yesterday'`
awk '{print "'$event' '$date'", $1}' | ./forward.py ${2:-localhost}
