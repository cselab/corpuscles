set -eu

. he.util

v="$1"
shift

off="$1"
shift

Ks="$1"
shift

Ka="$1"
shift

DA0D="$1"
shift

pi=3.141592653589793
volume=`off.volume $off`
area=`off.area   $off`
ta=`ae "3^(2/3)*4^(1/3)*$pi^(1/3)*$volume^(2/3)/$v^(2/3)"`
scale=`ae "($ta/$area)^(1/2)"`
off.scale $scale $off > ref.off

Kb=0.01
Kd=`ae 2/$pi*$Kb`

./main meyer_xin $v   0 4 1  ref.off skalak $Ks $Ka      $Kb 0 $Kd $DA0D < "$off" > q
