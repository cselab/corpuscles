set -eu

rV=0.63 Kb=0.1 C0=0 Kad=0.064
end=7500
for DAD0 in 20 22.5 27.5 30 35 40
do
    echo 2>&1 '************'  DAD0 = $DAD0 '************'
    ./main juelicher $rV   3 3 3 0       $Kb $C0 $Kad $DAD0          $end < ../../../data/rbc/laplace/0.off > q
    mv end.off $DAD0.off
done
