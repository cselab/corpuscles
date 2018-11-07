set -eu

. he.util
pi=3.141592653589793

off=$HOME/0.off
ref=$off

v=0.642
Ka=100 Kga=100 Kb=0.1 Ks=0 Kad=0 DA0D=0 C0=0

./main juelicher $v   $Ka $Kga $ref linear $Ks 0      $Kb $C0 $Kad $DA0D < $off '>' q
