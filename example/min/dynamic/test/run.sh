set -eu

. he.util

fa=1
Vr=0.642
Ka=$(ae 0.5*$fa)
Kga=$(ae 1*$fa)
Kv=$(ae 5*$fa)
Ke=0

Kb=$(ae 0.001*$fa)
C0=0
Kad=$(echo $Kb | awk '{print $1*0.63662}')
DA0D=0
D=0.000898798148042
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
#echo $A
end=1000000
freq=200

Da0p=0.11

(cd ../ && make )
if test $# -ne 0
then
    Da1="$1"
    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' Da1=$Da1 '***' Da0 = $Da0 '***'

    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    #echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D
	  
    if [ ! -d $Da1 ]; then
    mkdir $Da1
    fi
    cd $Da1
    off=../data/Nt1280_01.off

    ../../main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off > Da$Da1.msg
else
    bash run.sh $Da0p
    
fi
