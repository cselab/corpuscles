#!/bin/sh

set -eu

gdot=0.1
lambda=5
start=0
end=10000000
freq=500
dt=0.01
eta=645.928652122
rho=1
D=0.000898798148042

make
co.run ./main \
       garea 12.5663706144 1000 \
       volume 3.76991 5000 \
       area 0.0392699 500 \
       juelicher_xin 1 0 0 0 \
       1 $D $rho $eta $lambda $gdot $dt \
       $start $end $freq 200 \
       '<' init/0.9.off
