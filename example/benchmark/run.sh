#!/bin/sh

make main

la=5
start=0
end=1000
freq=100
#dt=0.00002
dt=0.000005
#VAL='--tool=callgrind --quiet' \
co.run ./main \
       garea 12.5663706144 1000000 \
       volume 2.6892 1000000 \
       juelicher_xin 1.0 0.0 0.63662 19.9933 \
       strain ref/0.off lim 278.521 139.260575205 -2 8 0.7 0.75 \
       1 0.000898798148042 1.0 645.928652122 $la \
       7.1961916509 \
       $dt $start $end $freq 50 '<' init/0.off
