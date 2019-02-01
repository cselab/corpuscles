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
D=0.000898798148042
Da0=0
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
#echo $A
nu=1
dt=0.001
kBT=$(echo $Kb | awk '{print $1/50.0}')

end=500000
freq=500

mus=0.139260575205
Kas=$(echo $mus | awk '{print $1*2}')
a3=-2
a4=8
b1=0.7
b2=0.75

off=$(he.path)/sph/laplace/Nt5120.off
ref=$(he.path)/sph/laplace/Nt5120.off

export HE_ARG="-W 24:00"

if test $# -ne 0
then
    Da1="$1"
    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' Da1=$Da1 '***' Da0 = $Da0 '***'

    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    #echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D
	  
    if [ ! -d nu${nu}_dt${dt}_${Da1} ]; then
	mkdir nu${nu}_dt${dt}_${Da1}
    fi
    cd nu${nu}_dt${dt}_${Da1}
    
    he.run ../../../main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $nu $dt $kBT $end $freq . strain $ref lim $Kas $mus $a3 $a4 $b1 $b2 '<' $off '>' Da$Da1.msg
else
    
    Da1=1.144
    bash run.sh $Da1
  

fi
