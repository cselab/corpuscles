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
eta_in=645.928652122
lambda=5.0
eta_out=$(echo ${eta_in} $lambda | awk '{print $1/$2}')
#echo "eta_in : "$eta_in
#echo "eta_out: "$eta_out
#exit  
gamdot0=0.00143923833018
dt0=0.01
tscale=10

start=0
end=20000000
freq_out=1000
freq_stat=100

#number of threads
NT=1
export CO_ARG="-W 120:00"
     
if test $# -ne 0
then

    num="$1"

    gamdot=$(echo $gamdot0, $num | awk '{print $1*$2}')
    dt=$(echo $dt0 $num $tscale | awk '{print $1*$3/$2}')

    if [ $num -le 100 ]; then
	Kc=$Kc0
    else
	Kc=$(echo $num $Kc0 | awk '{print $1*$2/100}')	
    fi

    Kga=$Kc
    Kv=$Kc

    Re=$(echo $gamdot $R $rho ${eta_out} | awk '{print $1*$2*$3/$4}')
    echo Re=$Re
    
    in_file=../init_tilt/init_v${v}_Da${Da1}.off
    ref_file=../ref/ref_v0.95.off
    
    echo "input file:" $in_file
    echo "ref file  :" $ref_file

    V=$(echo $A, $v | awk '{print 0.09403159725795977*$1**1.5*$2}')
    echo "A, V, v   :" $A, $V, $v
    
    Da0=$(echo $Da1 | awk '{print $1/100}')
    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print $4*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    
    flam=$(printf "%.1f" $lambda)
    fdt=$(printf "%.6f" $dt)

    echo "Da="$Da1
    echo "flam="$flam
    echo "num="$num
    echo "fdt="$fdt
    echo "Kc="$Kc

    folder="Da${Da1}_lam${flam}_num${num}_dt${fdt}_Kc${Kc}"
    
    if [ ! -d $folder ]; then
	mkdir $folder
    fi
    cd $folder
    
    co.run ../../main8 volume $V $Kv garea $A $Kga juelicher_xin $Kb $C0 $Kad $DA0D strain $ref_file lim $Ka $mub $a3 $a4 $b1 $b2 cortez_zero $R $D $rho ${eta_out} $lambda $gamdot $dt $start $end $freq_out $freq_stat '<' $in_file '1>/dev/null' '2>/dev/null'
      
    
else

    for i in `seq 2 2 10`;
    do
	num=$i
	bash run_dt10.sh $num
    done

    for i in `seq 20 10 200`;
    do
   	num=$i
	bash run_dt10.sh $num
    done

    for i in `seq 250 50 1000`;
    do
	num=$i
	bash run_dt10.sh $num
    done    

fi
    
