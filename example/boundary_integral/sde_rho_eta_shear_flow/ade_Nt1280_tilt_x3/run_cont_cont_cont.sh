set -eu

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
alpha=$(echo $pi | gawk '{print 2.0/$1}')
Kad=$(echo $Kb $alpha | gawk '{print $1*$2}')
D=0.000898798148042

mub=139.260575205
Ka=$(echo $mub | awk '{print $1*2}')
a3=-2
a4=8
b1=0.7
b2=0.75

Da1=0.143

rho=1.0
eta=645.928652122
start=0
end=9000000
freq_out=1000
freq_stat=100


lambda=1.0
gamdot0=0.00143923833018
gamdot1=0.00144
dt0=0.01
tscale=10

#number of threads
NT=1
export CO_ARG="-W 120:00"


if test $# -ne 0
then

    num="$1"
    
    if [ $num -le 200 ]; then
	Kc=$Kc0
    else
	Kc=$(echo $num $Kc0 | awk '{print $1*$2/100}')
    fi

    Kga=$Kc
    Kv=$Kc
    
    V=$(echo $A, $v | awk '{print 0.09403159725795977*$1**1.5*$2}')
    echo "A, V, v   :" $A, $V, $v

    Da0=$(echo $Da1 | awk '{print $1/100}')
    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print $4*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')

    ref_file=../ref/ref_v0.95.off
    
    Da1=$(printf "%.3f" $Da1)
    #echo "Da1="$Da1
    lam=$(printf "%.1g" $lambda)
    #echo "lam="$lam

    gamdot=$(echo $gamdot0, $num | awk '{print $1*$2}')
    gam=$gamdot
    gam=$(printf "%.4f" $gam)
    #echo "gam="$gam
    gam1=$(echo $gamdot1, $num | awk '{print $1*$2}')
    gam1=$(printf "%.3g" $gam1)
    #echo "gam1="$gam1

    dt=$(echo $dt0, $num, $tscale | awk '{print $1*$3/$2}')
    #echo "dt="$dt
    ddt=$(printf "%.4g" $dt)
    #echo "ddt="$ddt


    if [ -d Da${Da1}_lam${lam}_g${gam1}_dt${dt}_Kc${Kc}_cont_cont ]; then
	gam=$gam1;
    fi

    if [ -d  Da${Da1}_lam${lam}_g${gam1}_dt${ddt}_Kc${Kc}_cont_cont ]; then
	gam=$gam1;
	dt=$ddt;
    fi
    
    if [ -d  Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}_cont_cont ]; then
	dt=$ddt;
    fi
    
    echo "gam="$gam
    echo "dt="$dt

    folder="Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}_cont_cont"
    
    if [ ! -d ${folder}_cont ]; then
	mkdir ${folder}_cont
    fi    
    cd ${folder}_cont

    in_file=$(ls ../${folder}/*.off | tail -n 1)

    fname=`basename $in_file`
    fnum=${fname%.*}
    start=$(echo $fnum | sed 's/^0*//')

    echo "folder:  "$folder
    echo "in_file: "$in_file
    echo "start: "$start
    
    co.run ../../main garea $A $Kga volume $V $Kv juelicher_xin $Kb $C0 $Kad $DA0D strain $ref_file lim $Ka $mub $a3 $a4 $b1 $b2 $R $D $rho $eta $lambda $gamdot $dt $start $end $freq_out $freq_stat '<' $in_file '1>/dev/null' '2>/dev/null'

else
    
    num=1
    bash run_cont_cont_cont.sh $num

    num=2
    bash run_cont_cont_cont.sh $num


fi
