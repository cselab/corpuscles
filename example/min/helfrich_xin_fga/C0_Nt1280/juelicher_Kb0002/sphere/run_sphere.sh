set -eu

Vr=0.65
Kb=0.002
C0=0
Kad=0
DA0D=0
end=50000

off=../../../../../../data/sph/laplace/Nt1280.off

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main juelicher_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off > sphere_Vr$Vr.msg
    mv end.off sphere_Vr$Vr.off
    mv inter.dat sphere_Vr$Vr.dat
else
    for Vr in 0.7 0.65 0.6 0.55 0.5 0.45 0.4
    do sh run_sphere.sh $Vr
    done
fi
