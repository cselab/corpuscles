set -eu

if test $# -ne 0
then
    myfile="$1"
    echo $myfile
    off.rvolume $myfile
else
    for myfile in *.off;
    do
	bash check_oblate_Vr.sh $myfile
    done
fi
