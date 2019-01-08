if test $# -ne 0

then
    Da="$1"
    if [ -d $Da ]; then
	echo $Da
	echo "#et, eb, ea, ega, ev, ek, ee" >  eng_vol/Da$Da.eng
	awk '{ if ($1=="eng:") {print $2, $3, $4, $5, $6, $7, $8}}' $Da/stat.msg >> eng_vol/Da$Da.eng
	
	echo "#A/At, V/Vt, Vr" > eng_vol/Da$Da.vol
	awk '{ if ($3=="Vr:") {print  $4, $5, $6}}' $Da/stat.msg >> eng_vol/Da$Da.vol
    fi
else
    
    for i in `seq 0 10`;
    do
	Da=$(echo $i | awk '{print 0.1+$1*0.02}')
	#echo $Da
	bash extract_stat.sh $Da
    done
    
fi
