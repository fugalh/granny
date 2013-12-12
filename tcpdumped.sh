# pipe in "tcpdump -tttt -l ..."
# Each packet generates $event.

event=${1:-packet}
awk '{print "'$event'", $1, $2; fflush()}'
