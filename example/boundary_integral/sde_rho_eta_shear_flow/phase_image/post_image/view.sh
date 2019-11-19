
if [ $# -lt 1 ]; then
    echo "No off file specificed!"
    exit
else
    file="$1"    
fi

view=0
trans=1

if [ $# -eq 2 ]; then
    view="$2"
fi

if [ $# -eq 3 ]; then
    view="$2"
    trans="$3"
fi

#ref="../ref/ref_v0.95.off"
#ref="../ref/ref_rotate.off"
ref="../ref/ref_rotate_r.off"
#ref="../ref/ref_rotate_rr.off"

fo=25
sa=0.0
sb=1.0

WX=800 WY=600
if [ $view -eq 0 ]; then
    if [ $trans -eq 0 ]; then
	co.geomview -r 90 0 0 -f $fo -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" $file
    else
	co.geomview -r 90 0 0 -f $fo -a ~/app/app2a -f 30 -p "co.xcolor -a $ref $sa $sb" $file
    fi
elif [ $view -eq 1 ]; then	 
    if [ $trans -eq 0 ]; then
	co.geomview -r 0 0 0 -f $fo -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" $file
    else
	co.geomview -r 0 0 0 -f $fo -a ~/app/app2a -f 30 -p "co.xcolor -a $ref $sa $sb" $file
    fi
elif [ $view -eq 2 ]; then
    if [ $trans -eq 0 ]; then
	co.geomview -r 90 90 0 -f $fo -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" $file
    else
	co.geomview -r 90 90 0 -a ~/app/app2a -f $fo -p "co.xcolor -a $ref $sa $sb" $file
    fi
fi

