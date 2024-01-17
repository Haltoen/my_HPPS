#!/bin/sh
#
# Create a video of an n-body simulation. Set the variables FPS,
# FRAMES, STEPS_PER_FRAME, and NBODY to configure. Overwrites the
# provided particle file, so make sure it does not contain particles
# that are dear to you.
#
# This is a somewhat crude visualisation, and particles that move
# beyond the boundaries of the plot (have a magnitude of more than 2
# in any dimension) become invisible.

set -e # Die on error

if [ $# -ne 1 ]; then
    echo "Usage: $0 PARTICLES"
    exit 1
fi

PARTICLES=$1

FPS=24 # Frames per second
FRAMES=100 # Total frames
NBODY=./nbody # Simulation program
STEPS_PER_FRAME=1 # Steps done per frame

if ! which gnuplot >/dev/null; then
    echo "You must install gnuplot."
    exit 1
fi

if ! which ffmpeg >/dev/null; then
    echo "You must install ffmpeg."
    exit 1
fi

mkdir -p frames
rm -f frames/*.png

i=0
while [ $i -lt $FRAMES ]; do
    $NBODY $PARTICLES $PARTICLES $STEPS_PER_FRAME > /dev/null
    ./particles2text $PARTICLES > frames/particles.txt
    file=$(printf "frames/%.6d.png" $i)
    echo "$file"
    gnuplot -e 'inputfile="frames/particles.txt"' -e "outputfile='$file'" -c plot.gnu
    rm frames/particles.txt
    i=$(expr $i + 1)
done

ffmpeg -y -framerate $FPS -pattern_type glob -i 'frames/*.png' -c:v libx264 -pix_fmt yuv420p video.mp4
