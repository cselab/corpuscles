set -eu

. he.util
pi=3.141592653589793
off=$HOME/0.off

v=0.66
s=`ae "1/$v^(1/3)"`
off.scale $s $off > ref.off

Ka=0 Kga=10 Es=5.0 Ea=5e3   Kb=0.05 Kad=0 DA0D=0 C0=0

./main juelicher_xin $v   $Ka $Kga ref.off linear $Es $Ea      $Kb $C0 $Kad $DA0D < $off '>' q
