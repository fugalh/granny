The green slider on the left is the gain.

The yellow buttons will trigger events. (Run `granny -v` to see which events,
it depends on the contents of `wavs/`)

The horizontal blue slider is the grain duration. (Ranging from 0-100ms)

The horizontal red slider is the latency. (Ranging from 0-1s) If latency is
set too low, and events arrive late in spurts, you will get ear-splitting
clipping as simultaneous grains add up to astronomical volume.

TODO Find a way to prevent this ear splitting noise even when it happens. That
might save our hearing, but won't magically make it sound good. Or maybe we
can dynamically adjust latency based on how late things have been recently,
like in voip.
