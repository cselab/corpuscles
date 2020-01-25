#!/bin/sh

. co.util

export EPSREL=0.001 EPSABS=0.001
prog=run.sh

case x"$1" in
    x0) i=init/1280.off r=ref/1280.off ;;
    x1) i=init/5120.off r=ref/5120.off ;;
    xl) i=init/lubrication.off r=ref/1280.off ;;
    xl2) i=init/lubrication2.off r=ref/lubrication2.off ;;
    xa) i=init/amr.off r=ref/amr.off ;;
    *) echo >&2 "$0: unknown level: $1"
    exit 2
    ;;
esac

V=2.6892
kv=20000
A=12.5663706144
kga=20000
rho=1
eta=645.928652122
lambda=5
gamdot=0.576
start=0
end=99999999
freq_out=100
freq_stat=100
dt=`ae 0.05*$gamdot`
w=-1.5
make
co.run ./main volume $V $kv \
       garea $A $kga \
       juelicher_xin 1.0 0.0 0.63662 19.9933 \
       strain "$r" lim 278.521 139.260575205 -2 8 0.7 0.75 \
       wall $w \
       $rho $eta $lambda $gamdot $dt $start $end $freq_out $freq_stat \
       '<' "$i"
