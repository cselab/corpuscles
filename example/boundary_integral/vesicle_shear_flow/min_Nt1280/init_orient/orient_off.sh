end=200000.off
for i in `seq 0 9`;
do
    v=$(echo $i | awk '{print (0.5+$i*0.05)}')
    echo "orient v="$v
    orient < ../init/"init_v"$v.off > "init_v"$v.off
done

v=0.642
echo "orient v="$v
orient < ../init/"init_v"$v.off > "init_v"$v.off

v=0.98
echo "orient v="$v
orient < ../init/"init_v"$v.off > "init_v"$v.off
