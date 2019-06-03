set -eu

Nt=320
Kga=1000
Kv=5000
Ka=500

Kb=1.0

A=12.5663706144
V=2.68919777043
v=0.641998677173

gamdot=0.01
eta=645.928652122
dt=0.01
end=100000
freq=500

if test $# -ne 0
then

    v="$1"
    V=$(echo $A, $v | awk '{print 0.09403159725795977*$1**1.5*$2}')
    echo "A, V, v   :" $A, $V, $v
    in_file="../init_orient/init_v$v.off"
    echo "input file:" $in_file

    a=$(echo $A, $Nt | awk '{print $1/$2}')
    echo "a: ", $a
    
    if [ ! -d v${v}_g${gamdot}_dt${dt} ]; then
	
	mkdir v${v}_g${gamdot}_dt${dt}
	
    fi
    
    cd v${v}_g${gamdot}_dt${dt}
    
    ../../main garea $A $Kga volume $V $Kv area $a $Ka juelicher_xin $Kb 0 0 0 $gamdot $eta $dt $end $freq < $in_file
   
    
else
    
    #for i in `seq 0 9`;
    for i in `seq 0 0`;
    do
	v=$(echo $i | awk '{print (0.65+$i*0.05)}')
	bash run.sh $v
    done

fi
    
#./main garea $A 10 volume $V 10 strain $f lim 0.01 1 0 0 0 0  juelicher_xin 0.1 0 0 0 < $f
