axis=1
theta=0.785

echo "Rotating off file around axis $axis for $theta radian"
for fin in ../init_orient/*.off;
do
    fout=$(echo $fin | cut -d'/' -f 3)
    echo "from $fin to $fout"
    rotate $axis $theta < $fin > $fout
done
