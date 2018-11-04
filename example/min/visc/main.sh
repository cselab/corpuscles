set -eu

. he.util


: ${np=2}

DA0D=428
off=$HOME/e.off
v=0.642
pi=3.141592653589793
volume=`off.volume $off`
area=`off.area   $off`
ta=`ae "3^(2/3)*4^(1/3)*$pi^(1/3)*$volume^(2/3)/$v^(2/3)"`
scale=`ae "($ta/$area)^(1/2)"`

Kb=0.01
Kad=`ae 2/$pi*$Kb`
C0=0

for Ks in 0 1
do
    mkdir -p o/$Ks
    off.scale $scale $off > o/$Ks/ref.off
    (cd o/$Ks
     sem --tag --tagstring $Ks --lb -j $np --pipe \
	 juelicher juelicher $v   0.1 1 0.1  ref.off linear $Ks 0      $Kb $C0 $Kad $DA0D < $off '>' q
    )
done
