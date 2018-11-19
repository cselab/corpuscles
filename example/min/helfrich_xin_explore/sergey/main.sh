set -eu

make
#off=`he.path`/sph/laplace/Nt1280.off
off=0.off
end=999999999

freq=10000
./main meyer_xin 0.45 0.1 2 1 0 0.01 0 0 0 $end $freq < $off
