lambda=2.7
gamdot=0.01
dt=0.01

end=200000
freq=500
amount=100

if test $# -ne 0
then
    
    v="$1"

    lam=$(printf "%.1f" $lambda)
    echo "lam="$lam
    vv=$(printf "%.2f" $v)
    echo "vv="$vv
    gam=$(printf "%.2f" $gamdot)
    echo "gam="$gam
    ddt=$(printf "%.2f" $dt)
    echo "ddt="$ddt

    if [ -d ../lam${lam}_v${vv}_g${gam}_dt${ddt} ]; then

	echo "processing" ../lam${lam}_v${vv}_g${gam}_dt${ddt}"/..."
        bash post_single.sh ../lam${lam}_v${vv}_g${gam}_dt${ddt}/*.off > lam${lam}_v${vv}_time.dat
    fi

else
    
    for i in `seq 0 7`
    do
	v=$(echo $i | awk '{print (0.6+$i*0.05)}')
	bash post_time.sh $v
    done

fi

