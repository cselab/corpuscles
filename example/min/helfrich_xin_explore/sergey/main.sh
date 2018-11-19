set -eu

make
off=`he.path`/sph/laplace/Nt1280.off

./main meyer_xin 0.45 0.1 2 1 0 0.01 0 0 0 20000 200 < $off
