#!/bin/sh

. co.util

V=2.6892
kv=20000
A=12.5663706144
kga=20000
R=1
D=0.000898798148042
rho=1
eta=645.928652122
lambda=5
# gamdot=0.1152
gamdot=0.576
start=0
end=100
freq_out=10
freq_stat=100
dt=`ae 0.01*$gamdot`

make
co.run ./main volume $V $kv \
       garea $A $kga \
       juelicher_xin 1.0 0.0 0.63662 19.9933 \
       strain ref/ref_v0.95.off lim 278.521 139.260575205 -2 8 0.7 0.75 \
       cortez_zero \
       $R $D $rho $eta $lambda $gamdot $dt $start $end $freq_out $freq_stat \
       '<' init_tilt/init_v0.642_Da0.143.off
