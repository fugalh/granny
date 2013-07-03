[ -z "$1" ] && exit 1
rm *.wav
for i in $1/*.wav; do
    ln -fs $i .
done
