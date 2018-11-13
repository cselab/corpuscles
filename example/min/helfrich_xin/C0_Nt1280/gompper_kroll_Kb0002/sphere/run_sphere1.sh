set -eu

Vr=0.65
Kb=0.002
C0=0
Kad=0
DA0D=0
end=20000

off=../../../../../../data/sph/laplace/Nt1280.off

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../../main gompper $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off > sphere_Vr$Vr.msg
    mv end.off sphere_Vr$Vr.off
    mv inter.dat sphere_Vr$Vr.dat
else
    for Vr in 0.95 0.9 0.85 0.8 0.75 0.7
    do sh run_sphere1.sh $Vr
    done
fi