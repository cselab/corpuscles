for f in *.off
do
    echo "checking $f"
    off.rvolume $f
done
