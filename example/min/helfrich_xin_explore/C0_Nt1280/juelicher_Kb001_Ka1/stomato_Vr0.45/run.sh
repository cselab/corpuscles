set -eu

Kb=0.01
C0=0
Kad=0
DA0D=0
end=20000
freq=100

off=../obstart.off

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    if [ ! -d $Vr ]; then
	mkdir $Vr
    fi
    cd $Vr
    ../../../../main juelicher_xin $Vr 1 2 1 0 $Kb $C0 $Kad $DA0D $end $freq < $off > Vr$Vr.msg
else
    for i in `seq 1 13`;
    do
	Vr=$(echo $i | awk '{print 0.45+$1*0.02}')
	#echo $Vr
	bash run.sh $Vr
    done
fi
