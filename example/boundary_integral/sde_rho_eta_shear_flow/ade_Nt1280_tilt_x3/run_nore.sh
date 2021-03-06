set -eu

Da1=0.143
pi=3.141592653589793115997964
A=12.5663706144
V=2.68919777043
#v=0.641998677173
v=0.642
R=1

Nt=1280
Kc0=20000

Kb=1.0
C0=0.0
alpha=$(echo $pi | awk '{print 2.0/$1}')
Kad=$(echo $Kb $alpha | awk '{print $1*$2}')
D=0.000898798148042

mub=139.260575205
Ka=$(echo $mub | awk '{print $1*2}')
a3=-2
a4=8
b1=0.7
b2=0.75

rho=1.0
eta=645.928652122
lambda=1.0
gamdot0=0.00144
dt0=0.01
tscale=10

start=0
end=9000000
freq_out=1000
freq_stat=100

#number of threads
NT=1
export CO_ARG="-W 120:00"
     
if test $# -ne 0
then

    num="$1"

    gamdot=$(echo $gamdot0 $num | awk '{print $1*$2}')
    dt=$(echo $gamdot0 $dt0 $gamdot $tscale | awk '{print $1*$2/$3*$4}')

    if [ $num -le 100 ]; then
	Kc=$Kc0
    else
	Kc=$(echo $num $Kc0 | awk '{print $1*$2/100}');
    fi
    
    Kga=$Kc
    Kv=$Kc

    gam=$(printf "%.3g" $gamdot)
    echo "gam="$gam
    lam=$(printf "%.1g" $lambda)
    echo "lam="$lam
    dt=$(printf "%.4g" $dt)
    echo "dt="$dt
    #exit

    Re=$(echo $gamdot $R $rho $eta | awk '{print $1*$2*$3/$4}')
    echo Re=$Re
    
    in_file=../init_tilt/init_v${v}_Da${Da1}.off
    ref_file=../ref/ref_v0.95.off
    
    echo "input file:" $in_file
    echo "ref file  :" $ref_file

    V=$(echo $A, $v | awk '{print 0.09403159725795977*$1**1.5*$2}')
    echo "A, V, v   :" $A, $V, $v
    
    area=$(echo $A, $Nt | awk '{print $1/$2}')
    edge=$(echo $area | awk '{print 2*sqrt($1/sqrt(3.0))}')
    CFL=0.25
    dt_gam=$(echo $CFL $gamdot | awk '{print $1/$2}')
    echo "area, edge, dt_gam:" $area, $edge, $dt_gam

    Da0=$(echo $Da1 | awk '{print $1/100}')
    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print $4*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')

    if [ ! -d Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc} ]; then
	mkdir Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}
    fi
    cd Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}
    
    co.run ../../main garea $A $Kga volume $V $Kv juelicher_xin $Kb $C0 $Kad $DA0D strain $ref_file lim $Ka $mub $a3 $a4 $b1 $b2 $R $D $rho $eta $lambda $gamdot $dt $start $end $freq_out $freq_stat '<' $in_file
   
    
else

    for i in `seq 30 2 38`;
    do
	num=$i
	bash run_nore.sh $num
    done

    for i in `seq 42 2 48`;
    do
	num=$i
	bash run_nore.sh $num
    done

    for i in `seq 52 2 60`;
    do
	num=$i
	bash run_nore.sh $num
    done

    for i in `seq 65 5 75`;
    do
	num=$i
	bash run_nore.sh $num
    done

    for i in `seq 85 5 95`;
    do
	num=$i
	bash run_nore.sh $num
    done
    
    num=105
    bash run_nore.sh $num

    for i in `seq 110 10 190`;
    do
	num=$i
	bash run_nore.sh $num
    done


fi
    
