set -eu

make
off=`he.path`/sph/laplace/Nt1280.off
#off=0.off
end=999999999
freq=100

v=0.45
ka=1 kga=2 kv=1 ke=0.01 kb=0.01

MIX= ./main meyer_xin $v $ka $kga $kv $ke $kb 0 0 0 $end $freq < $off
