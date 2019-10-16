#!/bin/sh

set -eu

gdot=0.1
lambda=5.6
start=0
end=1000000
freq=10
dt=0.01
eta=645.928652122
rho=1
D=0.000898798148042

A=12.3165
V=3.6678

#init=init/0.off a=0.0384890625 reg=0.0301148
init=init/1.off a=0.009622265625 reg=0.0150574
#init=init/2.off a=0.00245436875 reg=1

make
co.run ./main \
       area $a 10 \
       garea $A 5000 \
       juelicher_xin 1 0 0 0 \
       cortez_zero \
       $rho $eta $lambda $gdot $dt \
       $start $end $freq 200 \
       '<' $init
