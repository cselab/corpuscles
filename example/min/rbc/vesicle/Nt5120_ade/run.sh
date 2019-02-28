set -eu

R=1.0
rho=1.0
v=0.641998677173
Ka=0
Kga=1000
Kv=5000
Ke=0

pi=3.141592653589793115997964
Kb=1
C0=0
Kad=$(echo  $pi | awk '{print 2.0/$1}')
DA0D=0
D=0.000898798148042
Da0=0
A=$(echo  $pi | awk '{print $1*4.0}')
xi=0.1
dt=0.00001
kBT=0.0001
end=200000
freq=500
freq_stat=100

mua=11.1408460164
mub=2.22816920329
a3=0
a4=0
b1=0
b2=0

off=$(he.path)/sph/icosa/Nt5120.off
ref=$(he.path)/sph/icosa/Nt5120.off

export HE_ARG="-W 24:00"

if test $# -ne 0
then
    v="$1"
    echo '***' v=$v '***'
        	  
    if [ ! -d v${v}_xi${xi}_kBT${kBT}_dt${dt} ]; then
	mkdir v${v}_xi${xi}_kBT${kBT}_dt${dt}
    fi
    cd v${v}_xi${xi}_kBT${kBT}_dt${dt}

    DA0D=1.112597
    he.run ../../../main juelicher_xin $R $rho $v $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $D $xi $dt $kBT $end $freq $freq_stat . strain $ref lim $mua $mub $a3 $a4 $b1 $b2 '<' $off '>' v$v.msg

else

    v=0.642
    bash run.sh $v

fi
