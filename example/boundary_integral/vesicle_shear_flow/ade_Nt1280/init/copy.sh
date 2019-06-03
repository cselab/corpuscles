end=200000.off
Da1=0.143

for i in `seq 0 9`;
do
    v=$(echo $i | awk '{print (0.5+$i*0.05)}')
    echo "copy v="$v
    cp ../../../../min/vesicle/ade_Nt1280/v${v}_Da1${Da1}/${end} init_v${v}_Da1${Da1}.off
done

v=0.52
echo "copy v="$v
cp ../../../../min/vesicle/ade_Nt1280/v${v}_Da1${Da1}/${end} init_v${v}_Da1${Da1}.off

v=0.642
echo "copy v="$v
cp ../../../../min/vesicle/ade_Nt1280/v${v}_Da1${Da1}/${end} init_v${v}_Da1${Da1}.off

v=0.98
echo "copy v="$v
cp ../../../../min/vesicle/ade_Nt1280/v${v}_Da1${Da1}/${end} init_v${v}_Da1${Da1}.off
