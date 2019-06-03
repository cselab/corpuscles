set -eu

v=0.642
Ka=500
Kga=1000
Kv=5000
Ke=0

Kb=1
C0=0
Kad=0
Kad=$(echo $Kb | awk '{print $1*0.63662}')
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
freq=1000

Da1=0.143


off=$(co.path)/sph/icosa/Nt320.off

export CO_ARG="-W 20:00"

if test $# -ne 0
then
    v="$1"

    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' v=$v '***' Da1 = $Da1 '***'

    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    #echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D

    if [ ! -d v${v}_Da1${Da1} ]; then
	
	mkdir v${v}_Da1${Da1}
	
    fi
    
    cd v${v}_Da1${Da1}
    
    co.run ../../main juelicher_xin $v $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $mu $dt $end $freq '<' $off '>' v$v.msg
else

    
    for i in `seq 0 9`;
    do
	v=$(echo $i | awk '{print (0.5+$i*0.05)}')
	echo "v="$v
	bash run.sh $v
    done

    v=0.52
    bash run.sh $v
    
    v=0.642
    bash run.sh $v

    v=0.98
    bash run.sh $v    
    
fi
