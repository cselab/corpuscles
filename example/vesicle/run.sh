#!/bin/sh

set -eu

gdot=0.1
lambda=1
start=0
end=100000
freq=1000
dt=0.001
eta=645.928652122
rho=1
D=0.000898798148042

A=12.3165
V=3.6678

init=init/fat.off a=0.0384890625 reg=0.0301148
#init=init/1.off a=0.0098174770425 reg=0.0150574
#init=init/2.off a=0.00245436875 reg=1

make
co.run ./main \
       garea $A  1000 \
       volume $V 5000 \
       area $a 500 \
       juelicher_xin 1 0 0 0 \
       cortez_fm \
       1 $D $rho $eta $lambda $gdot $dt \
       $start $end $freq 200 \
       '<' $init
