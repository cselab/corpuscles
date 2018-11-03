set -eu

. he.util

DA0D="$1"
shift

Ks="$1"
shift

off=$HOME/e.off
v=0.642
pi=3.141592653589793
volume=`off.volume $off`
area=`off.area   $off`
ta=`ae "3^(2/3)*4^(1/3)*$pi^(1/3)*$volume^(2/3)/$v^(2/3)"`
scale=`ae "($ta/$area)^(1/2)"`
off.scale $scale $off > ref.off

Kb=0.01
Kad=`ae 2/$pi*$Kb`
C0=0

./main juelicher $v   1 10 1  ref.off linear $Ks 0      $Kb $C0 $Kad $DA0D < $off > q
