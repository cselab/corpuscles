Kc=20000
#lambda=0.1754385964912281
#lambda=1.0
lambda=1.0

gamdot0=0.00143923833018
dt0=0.01

scale=10

every=0000
if test $# -ne 0
then
    Da1="$1"
    gamdot="$2"
    dt="$3"
    trans="$4"
    view="$5"
    Kc="$6"
    
    Da1=$(printf "%.3f" $Da1)
    echo "Da1="$Da1
    gam=$(printf "%.4f" $gamdot)
    echo "gam="$gam
    lam=$(printf "%.4f" $lambda)
    echo "lam="$lam
    dt=$(printf "%.6f" $dt)
    echo "dt="$dt

    #ref="../Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/0000000.off"
    #ref="../ref/ref_v0.95.off"
    ref="../ref/ref_rotate.off"
    sa=0.0
    sb=1.0

    if [ $trans -eq 1 ]; then
	if [ $view -eq 0 ]; then
	    WX=800 WY=600 co.geomview -r 90 0 0 -a ~/app/app2 -f 30 -p "co.xcolor $ref $sa $sb"  ../Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/*${every}.off
	else
	    WX=600 WY=800 co.geomview -r 0 0 90 -a ~/app/app2 -f 30 -p "co.xcolor $ref $sa $sb" ../Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/*${every}.off
	fi
    else
	if [ $view -eq 0 ]; then
	    WX=800 WY=600 co.geomview -r 90 0 0  -f 30 -p "co.xcolor $ref $sa $sb" ../Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/*${every}.off
	else
	    WX=600 WY=800 co.geomview -r 0 0 -90 -f 30 -p "co.xcolor $ref $sa $sb" ../Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/*${every}.off
	fi
    fi
    
else
    
    Da1=0.143
    
    trans=0
    view=0
    
    
    Kc=20000

    #gamdot=0.0014392383302
   
    #gamdot=0.00287847666036
    #gamdot=0.0071961916509
    
    #gamdot=0.014392383302 # 
    
    gamdot=0.0287847666036
    #lam=1 
    
    #gamdot=0.071961916509 #
    
    #gamdot=0.14392383302 #
    
    #gamdot=0.287847666036 Kc=40000
    #lam=5 rolling quadlobe
    
    #gamdot=0.71961916509 Kc=100000
    #lam=5 tank-treading
    
    #gamdot=1.4392383302 Kc=200000
    #lam=5 tank-treading
    
    #gamdot=2.87847666036 Kc=500000
    #lam=5 tank-treading
    
    #gamdot=7.1961916509 Kc=1000000
    #lam=5 tank-treading + rolling lobes

    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')

    bash view.sh $Da1 $gamdot $dt $trans $view $Kc

    
fi
