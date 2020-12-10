#!/bin/sh

set -eu
. co.util

ref=$1; shift
ini=$1; shift

pi=3.141592653589793115997964
LUNIT=3.3377e-6
a3=-2
a4=8
b1=0.7
b2=0.75
C0=0.0
m0bar=10
Kb=2.e-19
mub=2.5e-6

Ka=$(ae 2*$mub)

alpha=$(ae 2.0/$pi)
Kad=$(ae $Kb*$alpha)
DA0D=$(ae 2*$m0bar)

co.eng juelicher_xin $Kb 0 0 0 < "$ini"
co.eng juelicher_xin $Kb $C0 $Kad $DA0D < "$ini"
co.eng strain "$ref" lim $Ka $mub $a3 $a4 $b1 $b2 < "$ini"  | awk -v LU=$LUNIT '{print $1 * LU*LU}'
