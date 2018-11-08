set -eu

Vr=0.65
Kb=0.01
C0=0
Kad=0
DA0D=0
end=5000

off=../../../../../../data/oblate/laplace/Nt1280/oblate_area_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main meyer_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off$Vr.off > oblate_Vr$Vr.msg
    mv end.off oblate_Vr$Vr.off
    mv inter.dat oblate_Vr$Vr.dat
else
    for Vr in 0.7 0.65 0.6 0.55 0.5 0.45 0.4
    do sh run_oblate.sh $Vr
    done
fi
