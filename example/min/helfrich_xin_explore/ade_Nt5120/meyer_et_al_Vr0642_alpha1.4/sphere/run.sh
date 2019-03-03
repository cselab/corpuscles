set -eu

Vr=0.642
Ka=0.5
Kga=1
Kv=1
Ke=0

Kb=0.001
C0=0
Kad=$(echo $Kb | awk '{print $1*1.4}')
#echo $Kad
DA0D=0
D=0.000898798148042
Da0=0
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
#echo $A
end=200000
freq=500

off=$(co.path)/sph/laplace/Nt5120.off

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
    ../../../../main meyer_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off > Da$Da1.msg
else

    for i in `seq 0 5`;
    do
	Da1=$(echo $i | awk '{print (0.1+$1*0.02)}')
	#echo $Da1
	bash run.sh $Da1 &
    done

    for i in `seq 6 10`;
    do
	Da1=$(echo $i | awk '{print (0.1+$1*0.02)}')
	#echo $Da1
	bash run.sh $Da1 
    done

fi
