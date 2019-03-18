set -eu

R=1.0
rho=1.0
v=0.641998677173
Ka=0
Kga=1000
Kv=5000
Ke=0

Kb=1
C0=0
Kad=0
DA0D=0
D=0.000898798148042
Da0=0
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
xi=0.1
dt=0.000001
kBT=0.0
end=200000
freq=500

mua=11.1408460164
mub=2.22816920329
a3=0
a4=0
b1=0
b2=0

off=$(co.path)/sph/icosa/Nt1280.off
ref=$(co.path)/sph/icosa/Nt1280.off

export CO_ARG="-W 24:00"

if test $# -ne 0
then
    v="$1"
    echo '***' v=$v '***'
        	  
    if [ ! -d v${v}_xi${xi}_kBT${kBT}_dt${dt} ]; then
	mkdir v${v}_xi${xi}_kBT${kBT}_dt${dt}
    fi
    cd v${v}_xi${xi}_kBT${kBT}_dt${dt}
    
    co.run ../../../main juelicher_xin $R $rho $v $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $D $xi $dt $kBT $end $freq . strain $ref lim $mua $mub $a3 $a4 $b1 $b2 '<' $off '>' v$v.msg
else

    v=0.642
    bash run.sh $v

#  v=0.706
#   bash run.sh $v
    
#   v=0.732
#   bash run.sh $v

#   v=0.745
#   bash run.sh $v

#   v=0.758
#   bash run.sh $v
    
#   v=0.835
#   bash run.sh $v
    
#   v=0.95
#   bash run.sh $v

#   v=0.976
#   bash run.sh $v

#   v=0.989
#   bash run.sh $v

fi
