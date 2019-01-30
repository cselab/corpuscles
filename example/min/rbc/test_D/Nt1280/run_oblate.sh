set -eu

Vr=0.641998677173
Ka=0
Kga=1
Kv=5
Ke=0

Kb=0.001
C0=0
Kad=$(echo $Kb | awk '{print $1*0.636619772368}')
#echo $Kad
DA0D=0
D=0.000599198765361
Da0=0
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
#echo $A
end=200000
freq=500

mus=0.139260575205
Kas=$(echo $mus | awk '{print $1*2}')

a3=-2
a4=8
b1=0.7
b2=0.75

off=$(he.path)/oblate/laplace/Nt1280/oblate_area_Vr0.95.off
ref=$(he.path)/oblate/laplace/Nt1280/oblate_area_Vr0.95.off

export HE_ARG="-W 05:00"

if test $# -ne 0
then
    Da1="$1"
    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' Da1=$Da1 '***' Da0 = $Da0 '***'

    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    #echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D
	  
    if [[ "${Da1}" == -* ]]; then
	if [ ! -d m${Da1}_ob ]; then
	    mkdir m${Da1}_ob
	fi
    else
	if [ ! -d ${Da1}_ob ]; then
	    mkdir ${Da1}_ob
	fi
    fi
    
    if [[ "${Da1}" == -* ]]; then
	cd m${Da1}_ob
    else
	cd ${Da1}_ob
    fi
    
    he.run ../../main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq strain $ref lim $Kas $mus $a3 $a4 $b1 $b2 '<' $off '>' Da$Da1.msg
else

    Da1=-0.858
    bash run_oblate.sh $Da1 
    Da1=-0.358 
    bash run_oblate.sh $Da1 
    Da1=0.072
    bash run_oblate.sh $Da1 
    Da1=0.143
    bash run_oblate.sh $Da1 
    Da1=1.717
    bash run_oblate.sh $Da1 
    Da1=1.788
    bash run_oblate.sh $Da1 
    Da1=2.003
    bash run_oblate.sh $Da1 

fi
