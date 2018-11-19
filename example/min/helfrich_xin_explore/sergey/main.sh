set -eu

make
#off=`he.path`/sph/laplace/Nt1280.off
off=main.off

freq=10
./main meyer_xin 0.45 0.1 2 1 0 0.01 0 0 0 20000 $freq < $off
