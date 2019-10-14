for f in *.off
do
    echo "check file "$f
    echo "its area "$f
    off.area  $f
    echo "its volume "$f
    off.volume  $f
    echo "its reduced volume "$f
    off.rvolume  $f
done

