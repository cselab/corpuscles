#!/bin/sh

. co.util

prog=velocity.sh
if test $# -eq 0
then err 'missing input file'
fi
i=$1; shift
V=2.6892
kv=20000
A=12.5663706144
kga=20000
R=1
D=0.000898798148042
rho=1
eta=645.928652122
lambda=1
# gamdot=0.1152
gamdot=0.576

co.run ./velocity \
       garea $A $kga \
       juelicher_xin 1.0 0.0 0.63662 19.9933 \
       strain ref/ref_v0.95.off lim 278.521 139.260575205 -2 8 0.7 0.75 \
       cortez_zero \
       $eta $lambda $gamdot \
       '<' "$i"
