set -eu

Kb=0.01
C0=0
Kad=0
DA0D=0
end=200000
freq=500

off=$(he.path)/sph/laplace/Nt5120.off

#(cd ../../.. && make clean && make)

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    if [ ! -d $Vr ]; then
	mkdir $Vr
    fi
    cd $Vr
    ../../../../main gompper_xin $Vr 1 2 1 0 $Kb $C0 $Kad $DA0D $end $freq < $off > Vr$Vr.msg
else
    for i in `seq 0 5`;
    do
	Vr=$(echo $i | awk '{print 0.45-$1*0.05}')
	bash run.sh $Vr
    done
fi