set -eux

rV=0.642 Kb=0.1 C0=0 Kad=0.2546479089470325
end=2500
off=../../../../data/sph/laplace/0.off
np=2

main () (
    DAD0="$1"
    d="o/$1"
    echo 2>&1 '************'  DAD0 = $DAD0 '************'
    mkdir -p "$d"
    cp $off "$d"/main.off
    cd "$d"
    sem --tag --tagstring $DAD0 --lb -j $np --pipe meyer meyer_xin $rV   1 10 1 0       $Kb $C0 $Kad $DAD0          $end < main.off > q
)

if test $# -ne 0
then main "$@"
else for DAD0 in 28.00 29.60 31.20 32.80 34.40 36.00
     do sh main.sh $DAD0
     done
fi
