set -eux

rV=0.642 Kb=0.001 C0=0 Kad=0.0128
end=75000
off=../../../../data/sph/laplace/1.off
np=24

main () (
    DAD0="$1"
    d="o/$1"
    echo 2>&1 '************'  DAD0 = $DAD0 '************'
    mkdir -p "$d"
    cp $off "$d"/main.off
    cd "$d"
    sem --tag --tagstring $DAD0 --lb -j $np --pipe \
      meyer meyer_xin $rV   0.03 0.03 0.01 0 $Kb $C0 $Kad $DAD0 $end < main.off > q
)

if test $# -ne 0
then main "$@"
else for DAD0 in 28.00 28.35 28.70 29.04 29.39 29.74 30.09 30.43 30.78 31.13 31.48 31.83 32.17 32.52 32.87 33.22 33.57 33.91 34.26 34.61 34.96 35.30 35.65 36.00 
     do sh main.sh $DAD0
     done
fi
