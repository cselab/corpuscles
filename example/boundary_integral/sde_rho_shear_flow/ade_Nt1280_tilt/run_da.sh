set -eu

pi=3.141592653589793115997964
A=12.5663706144
V=2.68919777043
#v=0.641998677173
v=0.642
R=1

Nt=1280
Kc=20000
Kga=$Kc
Kv=$Kc

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
start=0
end=500000
freq_out=500
freq_stat=50

#number of threads
NT=1
export CO_ARG="-W 24:00"
     
if test $# -ne 0
then

    Da1="$1"
    gamdot="$2"
    dt="$3"
    
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

    gam=$(printf "%.4f" $gamdot)
    echo "gam="$gam
    if [ ! -d Da${Da1}_g${gam}_dt${dt}_Kc${Kc} ]; then
	
	mkdir Da${Da1}_g${gam}_dt${dt}_Kc${Kc}
	
    fi
    
    cd Da${Da1}_g${gam}_dt${dt}_Kc${Kc}
    
    co.run ../../main garea $A $Kga volume $V $Kv juelicher_xin $Kb $C0 $Kad $DA0D strain $ref_file lim $Ka $mub $a3 $a4 $b1 $b2 $R $D $rho $eta $gamdot $dt $start $end $freq_out $freq_stat '<' $in_file
   
    
else

    Da1=0.143
    dt=0.1

    gamdot=0.00143923833018
    bash run_da.sh $Da1 $gamdot $dt

    gamdot=0.00287847666036
    bash run_da.sh $Da1 $gamdot $dt

    gamdot=0.0071961916509
    bash run_da.sh $Da1 $gamdot $dt

    dt=0.01
    gamdot=0.0143923833018
    bash run_da.sh $Da1 $gamdot $dt

    gamdot=0.0287847666036
    bash run_da.sh $Da1 $gamdot $dt

    gamdot=0.071961916509
    bash run_da.sh $Da1 $gamdot $dt

    dt=0.001
    gamdot=0.143923833018
    bash run_da.sh $Da1 $gamdot $dt

    gamdot=0.287847666036
    bash run_da.sh $Da1 $gamdot $dt

    gamdot=0.71961916509
    bash run_da.sh $Da1 $gamdot $dt
    
    dt=0.0001
    gamdot=1.43923833018
    bash run_da.sh $Da1 $gamdot $dt

    gamdot=2.87847666036
    bash run_da.sh $Da1 $gamdot $dt

    gamdot=7.1961916509
    bash run_da.sh $Da1 $gamdot $dt

fi
    
