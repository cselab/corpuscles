set -eu

Vr=0.59
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
endp=200000
freq=500

if test $# -ne 0
then
    Da="$1"
    Dap="$2"
    echo '***' Da=$Da '***'
    
    DA0=$(echo $pi, $Da | awk '{print 4*$2*$1}')
    DA0D=$(echo $DA0 | awk '{print $1*2}')
    #echo $Da, $DA0, $DA0D
    
    if [ ! -d ${Da}_cont ]; then
	#echo ${Da}_cont
	mkdir ${Da}_cont
    fi
    cd ${Da}_cont
    
    off=../$Dap/$endp.off

    ../../../../main gompper_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off > Da$Da.msg
    
else

    bash run_cont.sh 1.2 1.18 &
    bash run_cont.sh 1.22 1.18 &
    bash run_cont.sh 1.25 1.3 &
    bash run_cont.sh 1.35 1.35 &
    
fi
