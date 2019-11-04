set -eu

v=0.642

if test $# -ne 0
then
    Da1=$1

    echo copying init_v${v}_Da${Da1}.off
    cp /Users/xinbian/codes/corpuscles/example/min/rbc_qpm_adapt/obdisco_obbicon/v095_Nt1280/sde_off_orient/final_v${v}_Da${Da1}.off init_v${v}_Da${Da1}.off

else

    Da1=-0.858
    bash copy.sh $Da1

    Da1=-0.358
    bash copy.sh $Da1

    Da1=0.072
    bash copy.sh $Da1

    Da1=0.143
    bash copy.sh $Da1

    Da1=1.717
    bash copy.sh $Da1

    Da1=1.788
    bash copy.sh $Da1

    Da1=2.003
    bash copy.sh $Da1
    
fi
