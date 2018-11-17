set -eu

if test $# -ne 0
then
    myfile="$1"
    echo $myfile
    off.area $myfile
    off.volume $myfile
    off.rvolume $myfile
else
    for myfile in *.off;
    do
	bash check_area_volume_vr.sh $myfile
    done
fi
