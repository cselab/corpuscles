set -eu

Vr=0.65
Kb=0.002
C0=0
Kad=0
DA0D=0
end=50000

off=../prolate_Vr0.45.off

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    #rm -r $Vr
    mkdir $Vr
    cd $Vr
    ../../../../main gompper $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off > stomato_Vr$Vr.msg &
    cd ..
else
    for Vr in 0.65 0.64 0.63 0.62 0.61 0.6 0.59 0.58 0.57 0.56 0.55 0.54 0.53 0.52 0.51 0.5 0.49 0.48
    do sh run_stomato.sh $Vr
    done
fi
