set -eu

Vr=0.65
Kb=0.002
C0=0
Kad=0
DA0D=0
end=50000

off=../sphere_Vr0.45.off

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    #rm -r $Vr
    mkdir $Vr
    cd $Vr
    ../../../../main meyer_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off > stomato_Vr$Vr.msg &
    #mv end.off oblate_Vr$Vr.off
    #mv inter.dat oblate_Vr$Vr.dat
    cd ..
else
    for Vr in 0.65 0.6 0.55 0.5 0.45 0.4 0.35 0.3 0.25
    do sh run_stomato.sh $Vr
    done
fi
