set -eu

. he.util
pi=3.141592653589793
#off=$HOME/0.off
off=$HOME/498.off

v=0.642
s=`ae "1/$v^(1/3)"`
off.scale $s $off > ref.off

Ka=10 Kga=10 Es=0 Ea=0   Kb=1e-3 Kad=`ae 0/$pi*$Kb` DA0D=0 C0=0
./main juelicher $v   $Ka $Kga ref.off skalak $Es $Ea      $Kb $C0 $Kad $DA0D < $off '>' q
