# read tcpdump on stdin and pretend all packets are gets. tcpdump doesn't give
# date, so we have to add one

# NB Run tcpdump with -l for realtime

event=${1:-get}
date=`date +%Y-%m-%d -d 'yesterday'`
awk '{print "'$event' '$date'", $1; fflush()}'
