#!/bin/bash
# Run in your "wavs" directory, with the path to the "theme" as the first
# argument. e.g. "mkdir wavs; cd wavs; ../data/theme.sh ../data/clangers"

dir=$1
[ -z "$dir" ] && exit 1

rm -f *.wav

for wav in "$dir/*.wav"; do
    ln -fs "$wav" .
done
