set -eu

off=../../../configuration_off_xin/sphere/icosa_ss.off

if test $# -ne 0
then
    Vr="$1"
    ../../../../../tri/example/sphere2spheroid/main $off oblate volume $Vr > oblate_volume_Vr$Vr.off
else
    for i in `seq 0 7`;
    do
	Vr=$(echo $i | awk '{print 0.05+$1*0.05}')
	#echo $Vr
	bash generate_oblate_fixed_volume.sh $Vr
    done
    
    for i in `seq 0 34`;
    do
	Vr=$(echo $i | awk '{print 0.45+$1*0.01}')
	#echo $Vr
	bash generate_oblate_fixed_volume.sh $Vr
    done
    
    for i in `seq 0 3`;
    do
	Vr=$(echo $i | awk '{print 0.8+$1*0.05}')
	#echo $Vr
	bash generate_oblate_fixed_volume.sh $Vr
    done
fi
