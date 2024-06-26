set -eu

Kb=0.002
C0=-2.4
Kad=0
DA0D=0
end=20000
freq=200

off=../../../../../../../data/prolate/laplace/Nt1280/prolate_area_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    if [ ! -d $Vr ]; then
	mkdir $Vr
    fi
    cd $Vr
    ../../../../main juelicher_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end $freq < $off$Vr.off > Vr$Vr.msg
else
    for i in `seq 0 15`;
    do
	Vr=$(echo $i | awk '{print 0.8-$1*0.02}')
	#echo $Vr
	bash run.sh $Vr
    done
fi
