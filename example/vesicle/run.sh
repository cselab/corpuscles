#!/bin/sh

set -eu

gdot=0.1
lambda=5
start=0
end=10000
freq=100
dt=0.01
eta=645.928652122
rho=1
D=0.000898798148042
A=12.5663706144

a=0.0392699
#a=0.0098174770425

make
co.run ./main \
       garea $A 1000 \
       volume 3.76991 5000 \
       area $a 500 \
       juelicher_xin 1 0 0 0 \
       1 $D $rho $eta $lambda $gdot $dt \
       $start $end $freq 200 \
       '<' init/0.off
