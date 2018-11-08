set -eu

Vr=0.65
Kb=0.001
C0=0
Kad=0
DA0D=0
end=100000

off=../../../../../../data/sph/laplace/Nt5120.off

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main juelicher_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off > sphere_Vr$Vr.msg
    mv end.off sphere_Vr$Vr.off
    mv inter.dat sphere_Vr$Vr.dat
else
    for Vr in 0.5 0.45 0.4 0.35 0.3 0.25 0.2
    do sh run_sphere1.sh $Vr
    done
fi
