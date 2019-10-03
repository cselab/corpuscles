lambda=1.0
num=50

lam=$(printf "%.1f" $lambda)
echo "lam="$lam
file_out=lam${lam}_av.dat

(
    echo "#v num mean variance standard deviation" 
    
for i in `seq 0 7`
do
    v=$(echo $i | awk '{print (0.6+$i*0.05)}')

    vv=$(printf "%.2f" $v)
    echo >&2 "vv="$vv
    
    if [ -f lam${lam}_v${vv}_time_cont.dat ]; then
	echo >&2 "processing " lam${lam}_v${vv}_time_cont.dat
	echo -n $vv " "
	tail -n $num lam${lam}_v${vv}_time_cont.dat > temp.out
	awk 'BEGIN{count=0; sum=0; sum2=0;} {count++; sum+=$1; sum2+=$1*$1;} END{sum/=count; sum2/=count; sum2-=(sum*sum); print count, sum, sum2, sqrt(sum2)}' temp.out  
	#rm temp.out
    fi
    
done

) > $file_out
