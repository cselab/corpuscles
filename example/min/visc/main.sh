set -eu

. he.util
pi=3.141592653589793

: ${np=2}

DA0D=0
off=$HOME/e.off

v=0.642
v0=`off.rvolume $off`
volume=`off.volume $off`
area=`off.area   $off`
ta=`ae "3^(2/3)*4^(1/3)*$pi^(1/3)*$volume^(2/3)/$v^(2/3)"`
scale=`ae "($ta/$area)^(1/2)*$v0^(1/3)"`

Ka=1
Kga=1
Kb=0
Kad=`ae 2/$pi*$Kb`
C0=0
Ks=0

off.scale $scale $off > ref.off
./main juelicher $v   $Ka $Kga ref.off linear $Ks 0      $Kb $C0 $Kad $DA0D < $off '>' q
