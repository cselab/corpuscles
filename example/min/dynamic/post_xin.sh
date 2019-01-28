set -eu
freq=10000
dt=0.005
mu=1
pi=3.14159265358979 

file="$1"

awk -v freq=$freq -v dt=$dt -v mu=$mu -v pi=$pi '{if (NR>1) {print (NR-2)*freq*dt/mu, $13/4/pi}}' $file
