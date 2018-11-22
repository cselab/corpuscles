set -eu

Ka=1
Kga=2
Kv=1
Ke=0

Kb=0.001
C0=0
Kad=0
DA0D=0

end=20000
freq=100

off=$(he.path)/prolate/laplace/Nt1280/prolate_area_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    if [ ! -d $Vr ]; then
	mkdir $Vr
    fi
    cd $Vr
    ../../../../main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off$Vr.off > Vr$Vr.msg
else
    for i in `seq 0 4`;
    do
	Vr=$(echo $i | awk '{print 0.95-$1*0.05}')
	#echo $Vr
	bash run.sh $Vr
    done
    for i in `seq 0 11`;
    do
	Vr=$(echo $i | awk '{print 0.73-$1*0.02}')
	#echo $Vr
	bash run.sh $Vr
    done
    for i in `seq 0 6`;
    do
	Vr=$(echo $i | awk '{print 0.5-$1*0.05}')
	#echo $Vr
	bash run.sh $Vr
    done

fi
