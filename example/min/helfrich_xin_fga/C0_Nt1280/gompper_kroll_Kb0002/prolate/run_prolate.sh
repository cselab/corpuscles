set -eu

Vr=0.65
Kb=0.002
C0=0
Kad=0
DA0D=0
end=50000

off=../../../../../../data/prolate/laplace/Nt1280/prolate_area_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main gompper $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off$Vr.off > prolate_Vr$Vr.msg
    mv end.off prolate_Vr$Vr.off
    mv inter.dat prolate_Vr$Vr.dat
else
    for Vr in 0.2 0.25 0.3 0.35 0.4 0.45 0.5 0.55 0.6 0.65 0.7 
    do sh run_prolate.sh $Vr
    done
fi
