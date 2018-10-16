set -eu

for DAD0 in 20 225 27.5 30 35 40
do
    echo 2>&1 '************'  DAD0 = $DAD0 '************'
    ./main juelicher 0.63   3 3 3 0       0.1 0 0.064 20          7500 < ../../../data/rbc/laplace/0.off > q
    mv end.off $DAD0.off
done
