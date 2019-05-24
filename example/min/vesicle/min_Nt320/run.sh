set -eu

v=0.642
Ka=500
Kga=1000
Kv=5000
Ke=0

Kb=1
C0=0
Kad=0
#echo $Kad
DA0D=0
D=0.000898798148042
Da0=0
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
#echo $A
mu=100
dt=0.0001
end=200000
freq=500


off=$(co.path)/sph/icosa/Nt320.off

export CO_ARG="-W 20:00"

if test $# -ne 0
then
    v="$1"
    
    if [ ! -d v${v} ]; then
	
	mkdir v${v}
	
    fi
    
    cd v${v}
    
    co.run ../../main juelicher_xin $v $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $mu $dt $end $freq '<' $off '>' v$v.msg
else

    #v=0.642
    #bash run.sh $v

    for i in `seq 0 9`;
    do
	v=$(echo $i  | awk '{print (0.5+$i*0.05)}')
	echo $v
    done
    
fi
