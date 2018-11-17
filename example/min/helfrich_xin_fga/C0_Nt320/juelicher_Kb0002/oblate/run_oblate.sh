set -eu

Kb=0.002
C0=0
Kad=0
DA0D=0
end=20000
freq=200
off=../../../../../../../data/oblate/icosa/Nt320/oblate_area_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    if [ ! -d $Vr ]; then
	mkdir $Vr
    fi
    cd $Vr
    ../../../../main juelicher_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end $freq < $off$Vr.off > oblate_Vr$Vr.msg
else
    for Vr in 0.95 0.85 0.75 0.7 0.65 0.6 0.55 0.5 0.45 0.4 0.35 0.3 0.25 0.2
    do sh run_oblate.sh $Vr
    done
fi
