for f in ../init/*.off
do
    echo "orient file "$f
    name=${f##*/}
    echo "name "$name
    orient < $f > $name
done

