set -eu

Vr=0.65
Kb=0.001
C0=0
Kad=0
DA0D=0
end=100000

off=../../../../../../data/prolate/laplace/Nt5120/prolate_volume_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main kantor $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off$Vr.off > prolate_Vr$Vr.msg
    mv end.off prolate_Vr$Vr.off
    mv inter.dat prolate_Vr$Vr.dat
else
    for Vr in 0.67 0.66 0.65 0.64 0.63 0.62 0.61 0.6 0.59 0.58 0.57 0.56 0.55
    do sh run_prolate.sh $Vr
    done
fi
