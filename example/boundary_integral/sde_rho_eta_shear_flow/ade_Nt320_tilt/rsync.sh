set -eu

Kc=20000
#lambda=0.1754385964912281
lambda=1.0
#lambda=5.0

gamdot0=0.00143923833018
dt0=0.01

scale=10

if test $# -ne 0
then

    Da1="$1"
    gamdot="$2"
    dt="$3"
    Kc="$4"
    
    Da1=$(printf "%.3f" $Da1)
    echo "Da1="$Da1
    gam=$(printf "%.4f" $gamdot)
    echo "gam="$gam
    lam=$(printf "%.4f" $lambda)
    echo "lam="$lam
    ddt=$(printf "%.6f" $dt)
    echo "dt="$dt

    if [ ! -d Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc} ]; then
	
	mkdir Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}
	
    fi
    
    cd Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}

    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_eta_shear_flow/ade_Nt320_tilt/Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}/*.off .
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_eta_shear_flow/ade_Nt320_tilt/Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}/msg.out .
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_eta_shear_flow/ade_Nt320_tilt/Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}/stat.dat .
    #rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_eta_shear_flow/ade_Nt320_tilt/Da${Da1}_lam${lam}_g${gam}_dt${ddt}_Kc${Kc}/lsf* .

    
else

    Da1=0.143
    
    gamdot=0.00143923833018
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=0.00287847666036
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=0.0071961916509
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=0.0143923833018
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=0.0287847666036
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=0.071961916509
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=0.143923833018
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=20000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=0.287847666036
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=40000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=0.71961916509
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=100000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=1.43923833018
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=200000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc
    
    gamdot=2.87847666036
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=500000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc

    gamdot=7.1961916509
    dt=$(echo $gamdot0 $dt0 $gamdot $scale | awk '{print $1*$2/$3*$4}')
    Kc=1000000
    bash rsync_da_dt_scale.sh $Da1 $gamdot $dt $Kc


    
fi
