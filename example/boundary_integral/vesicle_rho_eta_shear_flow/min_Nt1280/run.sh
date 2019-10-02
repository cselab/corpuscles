set -eu

R=1

Nt=1280
Kga=1000
Kv=5000
Ka=500

Kb=1.0
C0=0.0
alpha=0
Kad=0
D=0.000898798148042

A=12.5663706144
V=2.68919777043
v=0.641998677173


rho=1.0
eta=645.928652122
lambda=5.6
gamdot=0.01
dt=0.01
start=0
end=200000
freq_out=500
freq_stat=200

if test $# -ne 0
then

    v="$1"
    V=$(echo $A, $v | awk '{print 0.09403159725795977*$1**1.5*$2}')
    echo "A, V, v   :" $A, $V, $v

    a=$(echo $A, $Nt | awk '{print $1/$2}')
    echo "a: " $a

    lam=$(printf "%.1f" $lambda)
    echo "lam="$lam
    vv=$(printf "%.2f" $v)
    echo "vv="$vv
    gam=$(printf "%.2f" $gamdot)
    echo "gam="$gam
    ddt=$(printf "%.2f" $dt)
    echo "ddt="$ddt

    if [ ! -d lam${lam}_v${vv}_g${gam}_dt${ddt} ]; then
	
	mkdir lam${lam}_v${vv}_g${gam}_dt${ddt}
	
    fi
    
    cd lam${lam}_v${vv}_g${gam}_dt${ddt}

    in_file="../init_Nt1280/lam${lam}_v${vv}_g${gam}_dt${ddt}.off"
    echo "input file:" $in_file

    co.run ../../main garea $A $Kga volume $V $Kv area $a $Ka juelicher_xin $Kb 0 0 0 $R $D $rho $eta $lambda $gamdot $dt $start $end $freq_out $freq_stat '<' $in_file
    
    
else
    
    for i in `seq 0 7`;
    do
	v=$(echo $i | awk '{print (0.6+$i*0.05)}')
	bash run.sh $v
    done

fi
    
