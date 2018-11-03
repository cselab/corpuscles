set -eu

if test $# -ne 0
then
    Vr="$1"
    echo prolate_volume_Vr$Vr.off
    off.rvolume prolate_volume_Vr$Vr.off
else
    for Vr in 0.95 0.9 0.85 0.8 0.75 0.7 0.65 0.6 0.55 0.5 0.45 0.4 0.35 0.3 0.25 0.2 0.15 0.1 0.05
    do sh check_prolate_Vr.sh $Vr
    done
fi
