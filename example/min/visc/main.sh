set -eu

. he.util

DA0D="$1"
shift

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

for Ks in 0.00 4.35 8.70 13.04 17.39 21.74 26.09 30.43 34.78 39.13 43.48 47.83 52.17 56.52 60.87 65.22 69.57 73.91 78.26 82.61 86.96 91.30 95.65 100.00
do
    mkdir $Ks
    off.scale $scale $off > $Ks/ref.off
    (cd $Ks
     sem --tag --tagstring $Ks --lb -j 24 --pipe \
	 juelicher $v   1 10 1  ref.off linear $Ks 0      $Kb $C0 $Kad $DA0D < $off '>' q
    )
done
