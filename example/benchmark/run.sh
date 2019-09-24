#!/bin/sh

start=0
end=200

make main
co.run ./main \
       garea 12.5663706144 1000000\
       volume 2.6892 1000000\
       juelicher_xin 1.0 0.0 0.63662 19.9933\
       strain ref/main.off lim 278.521 139.260575205 -2 8 0.7 0.75 \
       1 0.000898798148042 1.0 645.928652122 5.0 \
       7.1961916509 0.00002 \
       $start $end 200 50 '<' init/main.off
