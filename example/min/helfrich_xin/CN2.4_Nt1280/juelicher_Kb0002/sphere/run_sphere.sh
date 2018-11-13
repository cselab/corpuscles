set -eu

Vr=0.65
Kb=0.002
C0=-2.4
Kad=0
DA0D=0
end=50000

off=../../../../../../data/sph/laplace/Nt1280.off

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main juelicher_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off >  Vr$Vr.msg
    mv end.off Vr$Vr.off
    mv inter.dat Vr$Vr.dat
else
    for Vr in 0.7 0.69 0.68 0.67 0.66 0.65 0.64 0.63 0.62 0.61 0.6 0.59 0.58 0.57 0.56 0.55 0.54 0.53 0.52 0.51 0.5
    do sh run_sphere.sh $Vr
    done
fi
