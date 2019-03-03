dt=0.005

m () {
    co.geomview -f 27 -r -130 0 0 -a ~/app/app42 -o /tmp/t.ppm $1
    convert -transparent 'rgb(255,255,255)' /tmp/t.ppm $2.png
}

for f in ../Nt_1280/dt_0.005/mu_1/[0-9]*.off
do b=$(basename $f .off)
   i=$(echo $b | sed 's,^0*,,1')
   i=$(echo $i | sed 's,^$,0,1')
   time=$(awk -v dt=$dt -v i=$i 'BEGIN {print int(i*dt)}')
   time=$(printf '%08d' $time)
   m $f $time
done
