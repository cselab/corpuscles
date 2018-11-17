set -eu

Vr=0.65
Kb=0.0005
C0=0
Kad=0
DA0D=0
end=50000

off=../../../../../../data/oblate/laplace/Nt20480/oblate_volume_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main meyer_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off$Vr.off > oblate_Vr$Vr.msg
    mv end.off oblate_Vr$Vr.off
    mv inter.dat oblate_Vr$Vr.dat
else
    for Vr in 0.45 0.4 0.35 0.3 0.25
    do sh run_oblate.sh $Vr
    done
fi
