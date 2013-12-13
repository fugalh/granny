#!/usr/bin/env python
import granny

import sys
import time
import traceback


host = 'localhost'
port = 1337
latency = 1 # seconds

if len(sys.argv) > 1:
    host = sys.argv[1]
if len(sys.argv) > 2:
    port = int(sys.argv[2])
gran = granny.Client((host, 1337))

first = None
now = time.time()
while True:
    # Can't do "for line in sys.stdin" because of buffering
    line = sys.stdin.readline()
    if line == "":
        break

    try:
        args = line.strip().split()
        if len(args) != 1 and len(args) != 3:
            continue

        what = args[0]

        when = None
        if len(args) > 1:
            # parse when according to ngrep timestamp format
            when = " ".join(args[1:])

            micros = 0
            if '.' in when:
                when, micros = when.split('.', 1)

            # ngrep uses slashes. tsk tsk
            when = when.replace('/', '-')

            when = time.mktime(time.strptime(when, "%Y-%m-%d %H:%M:%S"))
            when += float(micros) / 1e6

            # Time shift so that the first event aligns with now
            if first is None:
                if when < now:
                    first = now - when
                else:
                    first = 0
            when += first

        gran.send(what, when)

    except OSC.OSCClientError:
        raise
    except:
        traceback.print_exc()

# TODO
# proper options (host, port, latency, strptime format)
# more robust parsing/error recovery
# bundling
