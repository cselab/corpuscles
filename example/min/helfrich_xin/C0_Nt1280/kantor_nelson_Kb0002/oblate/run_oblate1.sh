set -eu

Vr=0.65
Kb=0.002
C0=0
Kad=0
DA0D=0
end=20000

off=../../../../../../data/oblate/laplace/Nt1280/oblate_volume_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main kantor $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off$Vr.off > oblate_Vr$Vr.msg
    mv end.off oblate_Vr$Vr.off
    mv inter.dat oblate_Vr$Vr.dat
else
    for Vr in 0.95 0.9 0.85 0.8 0.75 0.7
    do sh run_oblate1.sh $Vr
    done
fi
