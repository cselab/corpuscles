#!/bin/sh

. co.util

#lambda=0.08 Ca=0.4
#lambda=1 Ca=0.38
#lambda=3.6 Ca=1.5
lambda=6.4 Ca=2.5

area=12.5065
gdot=`ae 2*$Ca*$area`
start=0
end=1000000
freq=100
dt=0.001
eta=1
rho=1

A=12.3165
V=3.6678
sigma=1

#init=`co.path`/sph/icosa/Nt320.off
init=`co.path`/sph/icosa/Nt1280.off

make
co.run ./main \
       area_diff 0.04 \
       garea_sq $sigma \
       cortez_zero \
       $rho $eta $lambda $gdot $dt \
       $start $end $freq 200 \
       '<' $init
