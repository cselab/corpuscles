#!/bin/sh

set -eu

gdot=0.1
lambda=5.6
start=0
end=1000000
freq=100
dt=0.01
eta=645.928652122
rho=1
D=0.000898798148042

A=12.3165
V=3.6678

#init=init/0.off a=0.0384890625
#init=init/1.off a=0.009622265625
init=init/2.off a=0.00240556640625

make
co.run ./main \
       area $a 400 \
       garea $A 50000 \
       juelicher_xin 1 0 0 0 \
       cortez_zero \
       $rho $eta $lambda $gdot $dt \
       $start $end $freq 200 \
       '<' $init
