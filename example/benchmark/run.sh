#!/bin/sh

make main

la=0.5
start=0
end=200
freq=1000
#valgrind --tool=callgrind --quiet \
./main \
       garea 12.5663706144 1000000 \
       volume 2.6892 1000000 \
       juelicher_xin 1.0 0.0 0.63662 19.9933 \
       strain ref/main.off lim 278.521 139.260575205 -2 8 0.7 0.75 \
       1 0.000898798148042 1.0 645.928652122 $la \
       7.1961916509 0.00002 \
       $start $end $freq 50 < init/main.off
