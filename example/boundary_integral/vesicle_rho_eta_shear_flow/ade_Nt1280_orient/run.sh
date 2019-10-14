set -eu

pi=3.141592653589793115997964
A=12.5663706144
V=2.68919777043
#v=0.641998677173
v=0.642
R=1

Nt=320
Kc=20000
Kga=$Kc
Kv=$Kc
Ka=500

Kb=1.0
C0=0.0
alpha=$(echo $pi | gawk '{print 2.0/$1}')
Kad=$(echo $Kb $alpha | gawk '{print $1*$2}')
D=0.000898798148042

rho=1.0
eta=645.928652122
lambda=1.0
start=0
end=9000000
freq_out=1000
freq_stat=200

gamdot0=0.00143923833018
dt0=0.01

scale=10
#number of threads
NT=1
export CO_ARG="-W 48:00"

if test $# -ne 0
then

    Da1="$1"
    gamdot="$2"
    dt="$3"
    Kc="$4"
    
    Kga=$Kc
    Kv=$Kc

    Re=$(echo $gamdot $R $rho $eta | awk '{print $1*$2*$3/$4}')
    echo Re=$Re

    in_file=../init_orient/init_v${v}_Da${Da1}.off
    echo "input file:" $in_file

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


    Da1=$(printf "%.3f" $Da1)
    echo "Da1="$Da1
    gam=$(printf "%.4f" $gamdot)
    echo "gam="$gam
    lam=$(printf "%.4f" $lambda)
    echo "lam="$lam
    ddt=$(printf "%.6f" $dt)
    echo "dt="$dt

    a=$(echo $A, $Nt | awk '{print $1/$2}')
    echo "a: " $a


    if [ ! -d Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc} ]; then
	
	mkdir Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}
	
    fi
    
    cd Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}

    co.run ../../main garea $A $Kga volume $V $Kv area $a $Ka juelicher_xin $Kb $C0 $Kad $DA0D $R $D $rho $eta $lambda $gamdot $dt $start $end $freq_out $freq_stat '<' $in_file
    
    
else
    
    Da1=0.143
    
    gamdot=0.00143923833018
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash run.sh $Da1 $gamdot $dt $Kc
    
    gamdot=0.00287847666036
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=0.0071961916509
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=0.0143923833018
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=0.0287847666036
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=0.071961916509
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=0.143923833018
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=0.287847666036
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=40000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=0.71961916509
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    echo "dt="$dt
    Kc=100000
    bash run.sh $Da1 $gamdot $dt $Kc
    
    gamdot=1.43923833018
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=200000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=2.87847666036
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=500000
    bash run.sh $Da1 $gamdot $dt $Kc

    gamdot=7.1961916509
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=1000000
    bash run.sh $Da1 $gamdot $dt $Kc

fi
    
