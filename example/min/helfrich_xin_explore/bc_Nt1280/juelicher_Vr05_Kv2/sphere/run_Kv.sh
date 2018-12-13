set -eu

Vr=0.5
Ka=0.5
Kga=1
Kv=5
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
    
    if [ ! -d ${Da}_Kv ]; then
	#echo ${Da}_Kv
	mkdir ${Da}_Kv
    fi
    cd ${Da}_Kv
    off=../${Da}/$end.off
    ../../../../main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off > Da$Da.msg
else

    bash run_Kv.sh 1.3
    bash run_Kv.sh 1.35
    bash run_Kv.sh 1.4
    bash run_Kv.sh 1.45

fi
