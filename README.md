# granny: event-based granular synthesis
Granny is a granular synthesis engine for auralizing events. e.g. network
traffic, web or memcache requests, swap paging, disk accesses, etc. Anything
that happens frequently is a good candidate. This auralization is not only fun
and interesting and perhaps even musical, but can be very informative for
gaining insight into the distribution of events. Humans are excellent pattern
matchers. Time series of many events is not easy to convey visually, but it is
very natural indeed when conveyed aurally.

## Requirements
- [jack](http://jackaudio.org/)
- [liblo](http://liblo.sourceforge.net/)
- [libsndfile](http://www.mega-nerd.com/libsndfile/)
- [ØMQ](http://zeromq.org/)

## Grains
Make >100ms wav files and put them in `wavs/`.
(Must be 44.1kHz for now. Stereo is ok, but we only use the first channel.)

Good source material might include sounds from your favorite softsynth,
snippets from your favorite song(s) or movies, recordings of random strangers
at the train station...

Grains will be generated from these source files, with random start times and
with a duration and window as requested.

## Events
Granny will respond to OSC events in the form `/event/$(basename wavs/*.wav)`, e.g.

    /event/www
    /event/smtp
    /event/dns

## Bundles
Granny recognizes OSC bundles, and honors bundle timestamps. Some internal latency
is added to account for processing/network latency (TODO a command-line option
to control this). Bundles with timestamps in the past are time-shifted to the
present based on the first timestamp seen. (This also happens for streams that
appear to be in the future, due perhaps to clock skew or timezone mismatch.)
This is handled on a per-connection basis. This makes it easy to generate the
same event stream every time from a packet capture - just use the actual
timestamps.

## Parameters
Granny also responds to these control messages:

- /control/gain f (gain from 0-1)
- /control/duration i (grain duration in milliseconds)
- /control/envelope s (name of envelope - see Envelope.hh)

These are also initializable from the command line.  Run `granny -h` for usage.

## UI
There is a [TouchOSC](http://hexler.net/software/touchosc) UI for controlling
the control parameters in `ui/`. TouchOSC isn't free but it's worth the $5 for
your iOS or Android device. If you need a free app, try
[Control](http://charlie-roberts.com/Control/), but it's more work to set up a
UI. I'm interested in a GUI and/or controlling via a MIDI control surface (I
use OS X but contributions for any platform are welcome.)
