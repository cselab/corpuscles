set -eu

Vr=0.31
Ka=0.5
Kga=1
Kv=1
Ke=0

Kb=0.001
C0=0
Kad=1
#echo $Kad
DA0D=0
D=0.000898798148042
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
#echo $A
end=200000
freq=500


if test $# -ne 0
then
    Da="$1"
    echo '***' Da=$Da '***'
    
    DA0=$(echo $pi, $Da | awk '{print 4*$2*$1}')
    DA0D=$(echo $DA0 | awk '{print $1*2}')
    #echo $Da, $DA0, $DA0D
    
    if [ ! -d ${Da}_refine ]; then
	#echo ${Da}_refine
	mkdir ${Da}_refine
    fi
    cd ${Da}_refine
    off=Nt20480_start.off
    ../../../../main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off > Da$Da.msg
else
    for i in `seq 0 8`;
    do
	Da=$(echo $i | awk '{print (1.8+$1*0.05)}')
	#echo $Da
	bash run_refine.sh $Da &
    done

fi
