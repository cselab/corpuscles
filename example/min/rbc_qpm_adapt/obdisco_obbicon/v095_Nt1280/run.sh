set -eu
pi=3.141592653589793115997964

R=1.0
rho=1.0
v=0.641998677173
A=$(echo  $pi | awk '{print $1*4.0}')

Kc=1000
freqk=100
coefk=1.1

Kb=1
C0=0
alpha=$(echo $pi | awk '{print 2.0/$1}')
Kad=$(echo $Kb $alpha | awk '{print $1*$2}')
DA0D=0
D=0.000898798148042
Da0=0

xi=0.1
dt=0.000001
kBT=0.0

mub=139.260575205
Ka=$(echo $mub | awk '{print $1*2}')
a3=-2
a4=8
b1=0.7
b2=0.75

end=300000
freqo=1000
freqs=100

ref=../ref/v095.off
init=../init/init.off

export CO_ARG="-W 24:00"

if test $# -ne 0
then
    Da1="$1"
    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' Da1=$Da1 '***' Da0 = $Da0 '***'

    #DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print $4*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    #echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D

    #create a folder to run and folder name consists of parameters
    if [ ! -d Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1} ]; then
	mkdir Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1}
    fi
    cd Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1}
    
    co.run ../../../main $R $rho $v $Kc $freqk $coefk \
    juelicher_xin $Kb $C0 $Kad $DA0D \
    $D $xi $dt $kBT \
    strain $ref lim $Ka $mub $a3 $a4 $b1 $b2 \
    $end $freqo $freqs '<' $init
    
else

    Da1=-0.858
    bash run.sh $Da1

    Da1=-0.358
    bash run.sh $Da1

    Da1=0.072
    bash run.sh $Da1

    Da1=0.143
    bash run.sh $Da1

    Da1=1.717
    bash run.sh $Da1

    Da1=1.788
    bash run.sh $Da1

    Da1=2.003
    bash run.sh $Da1
    
fi
