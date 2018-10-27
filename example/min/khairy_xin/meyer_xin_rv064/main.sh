set -eu

rV=0.641998677173
Kb=0.1
C0=0
Kad=0.0636619772368
end=2500
off=../../../../data/sph/laplace/0.off

if test $# -ne 0
then
    DA0D="$1"
    echo 2>&1 '************'  DA0D = $DA0D '************'
    ./main meyer_xin $rV 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off > q
    mv end.off $DA0D.off
else
    for DA0D in 12.06 14.47 16.89 19.30 21.71 24.12 26.53 28.95 31.36 33.77 36.18
    do sh main.sh $DA0D
    done
fi
