set -eu

rV=0.642 Kb=0.1 C0=0 Kad=0.06366197723675814
end=2500
off=../../../../data/sph/laplace/1.off

if test $# -ne 0
then
    DAD0="$1"
    echo 2>&1 '************'  DAD0 = $DAD0 '************'
    ./main juelicher $rV   3 3 3 0       $Kb $C0 $Kad $DAD0          $end < $off > q
    mv end.off $DAD0.off
else
    for DAD0 in 20 22.5 25 27.5 28.5 29.25 30 31.25 33.125 32.5 32.8125 33.75 35 40
    do sh main.sh $DAD0
    done
fi
