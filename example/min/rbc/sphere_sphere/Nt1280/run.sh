set -eu

R=1.0
rho=1.0
v=0.641998677173
#Ka=500
Ka=0
Kga=1000
Kv=5000
Ke=0

Kb=1
C0=0
Kad=$(echo $Kb | awk '{print $1*0.636619772368}')
DA0D=0
D=0.000898798148042
Da0=0
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
xi=0.1
dt=0.000001
kBT=0.0
end=200000
freq=1000

mub=139.260575205
mua=$(echo $mub | awk '{print $1*2}')
#a3=-2
#a4=8
#b1=0.7
#b2=0.75
a3=0
a4=0
b1=0
b2=0

off=$(co.path)/sph/icosa/Nt1280.off
ref=$(co.path)/sph/icosa/Nt1280.off

export HE_ARG="-W 24:00"

if test $# -ne 0
then
    Da1="$1"
    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' Da1=$Da1 '***' Da0 = $Da0 '***'

    #DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print $4*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    #echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D
	  
    if [ ! -d xi${xi}_kBT${kBT}_dt${dt}_Da${Da1} ]; then
	mkdir xi${xi}_kBT${kBT}_dt${dt}_Da${Da1}
    fi
    cd xi${xi}_kBT${kBT}_dt${dt}_Da${Da1}
    
    
    co.run ../../../main juelicher_xin $R $rho $v $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $D $xi $dt $kBT $end $freq . strain $ref lim $mua $mub $a3 $a4 $b1 $b2 '<' $off 
else
    
    Da1=1.144
    bash run.sh $Da1

fi
