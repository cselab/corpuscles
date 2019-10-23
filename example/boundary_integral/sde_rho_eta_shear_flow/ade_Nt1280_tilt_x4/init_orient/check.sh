v=0.642

if test $# -ne 0
then
    Da1="$1"
    echo checking init_v${v}_Da${Da1}.off
    echo -n "volume:  "
    off.volume  init_v${v}_Da${Da1}.off
    echo -n "area:    "
    off.area    init_v${v}_Da${Da1}.off
    echo -n "rvolume: "
    off.rvolume init_v${v}_Da${Da1}.off

else
    Da1=-0.858
    bash check.sh $Da1

    Da1=-0.358
    bash check.sh $Da1

    Da1=0.072
    bash check.sh $Da1

    Da1=0.143
    bash check.sh $Da1

    Da1=1.717
    bash check.sh $Da1

    Da1=1.788
    bash check.sh $Da1

    Da1=2.003
    bash check.sh $Da1

fi
