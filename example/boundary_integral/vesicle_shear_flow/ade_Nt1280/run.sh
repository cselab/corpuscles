set -eu

Nt=320
Kga=1000
Kv=5000
Ka=500

Kb=1.0
C0=0
#Kad=0
Kad=$(echo $Kb | awk '{print $1*0.63662}')
#echo $Kad
DA0D=0
D=0.000898798148042
Da0=0
pi=3.141592653589793115997964

A=12.5663706144
V=2.68919777043
v=0.641998677173

gamdot=0.01
eta=645.928652122
dt=0.01
end=100000
freq=500

Da1=0.143

if test $# -ne 0
then

    v="$1"
    V=$(echo $A, $v | awk '{print 0.09403159725795977*$1**1.5*$2}')
    echo "A, V, v   :" $A, $V, $v

    a=$(echo $A, $Nt | awk '{print $1/$2}')
    echo "a: ", $a

    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' v=$v '***' Da1 = $Da1 '***'

    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D

    in_file=../init_orient/init_v${v}_Da1${Da1}.off
    echo "input file:" $in_file

    if [ ! -d v${v}_Da1${Da1}_g${gamdot}_dt${dt} ]; then
	
	mkdir v${v}_Da1${Da1}_g${gamdot}_dt${dt}
	
    fi
    
    cd v${v}_Da1${Da1}_g${gamdot}_dt${dt}
    
    co.run ../../main garea $A $Kga volume $V $Kv area $a $Ka juelicher_xin $Kb $C0 $Kad $DA0D $gamdot $eta $dt $end $freq '<' $in_file
   
    
else
    
    for i in `seq 0 9`;
    do
	v=$(echo $i | awk '{print (0.5+$i*0.05)}')
	bash run.sh $v
    done

    v=0.52
    bash run.sh $v

    v=0.642
    bash run.sh $v

    v=0.98
    bash run.sh $v

fi
