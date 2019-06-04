set -eu

pi=3.141592653589793115997964
A=12.5663706144
V=2.68919777043
#v=0.641998677173
v=0.642
R=$(echo $A $pi | awk '{print sqrt($1/$2/4)}')
echo R=$R

Nt=320
Kga=1000
Kv=5000
Ka=500

Kb=1.0
C0=0.0
Kad=$(echo $Kb | awk '{print $1*0.636619772368}')
D=0.000898798148042

Da1=0.143

Da0=$(echo $Da1 | awk '{print $1/100}')
DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print $4*$7}')
DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
   
mub=139.260575205
mua=$(echo $mub | awk '{print $1*2}')
a3=-2
a4=8
b1=0.7
b2=0.75

gamdot=0.1
eta=645.928652122
rho=1.0
dt=0.01
end=100000
freq=500

     
if test $# -ne 0
then

    gamdot="$1"

    Re=$(echo $gamdot $R $rho $eta | awk '{print $1*$2*$3/$4}')
    echo Re=$Re

    V=$(echo $A, $v | awk '{print 0.09403159725795977*$1**1.5*$2}')
    echo "A, V, v   :" $A, $V, $v
    in_file=../init_orient/init_v${v}_Da${Da1}.off
    ref_file=../ref/ref_v0.95.off
    
    echo "input file:" $in_file

    a=$(echo $A, $Nt | awk '{print $1/$2}')
    echo "a: ", $a
    
    if [ ! -d v${v}_Da${Da1}_g${gamdot}_dt${dt} ]; then
	
	mkdir v${v}_Da${Da1}_g${gamdot}_dt${dt}
	
    fi
    
    cd v${v}_Da${Da1}_g${gamdot}_dt${dt}
    
    ../../main garea $A $Kga volume $V $Kv area $a $Ka juelicher_xin $Kb $C0 $Kad $DA0D strain $ref_file lim $mua $mub $a3 $a4 $b1 $b2 $gamdot $eta $dt $end $freq < $in_file
   
    
else

    gamdot=0.5
    bash run.sh $gamdot
    
fi
    
