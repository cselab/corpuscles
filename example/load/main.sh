make

# tri.plain 20 12 > ref.off
cp data/main.off ref.off

dt=1e-3
Ka=1
mu=1
nstep=1000000
he.run ./main $dt $nstep strain ref.off lim $Ka $mu 0 0 0 0 edge 1 < ref.off  > o.off
