set -eu

lambda=0.1754385964912281
lambda=5.0

Kc=20000

if test $# -ne 0
then

    Da1="$1"
    lambda="$2"
    gamdot="$3"
    dt="$4"
    Kc="$5"
    
    lam=$(printf "%.4f" $lambda)
    echo "lam="$lam
    gam=$(printf "%.4f" $gamdot)
    echo "gam="$gam
    if [ ! -d Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc} ]; then
	
	mkdir Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}
	
    fi
    
    cd Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}

    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_eta_shear_flow/ade_Nt120_tilt/Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/*.off .
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_eta_shear_flow/ade_Nt120_tilt/Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/msg.out .
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_eta_shear_flow/ade_Nt120_tilt/Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/stat.dat .
    #rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_eta_shear_flow/ade_Nt120/Da${Da1}_lam${lam}_g${gam}_dt${dt}_Kc${Kc}/lsf* .

    
else

    Da1=0.143

    
    gamdot=0.00143923833018
    dt=0.01
    Kc=20000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=0.00287847666036
    dt=0.005
    Kc=20000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=0.0071961916509
    dt=0.002
    Kc=20000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=0.0143923833018
    dt=0.001
    Kc=20000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=0.0287847666036
    dt=0.0005
    Kc=20000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=0.071961916509
    dt=0.0002
    Kc=20000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=0.143923833018
    dt=0.0001
    Kc=20000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=0.287847666036
    dt=0.00005
    Kc=40000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=0.71961916509
    dt=0.00002
    Kc=100000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=1.43923833018
    dt=0.00001
    Kc=200000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc
    
    gamdot=2.87847666036
    dt=0.000005
    Kc=500000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc

    gamdot=7.1961916509
    dt=0.000002
    Kc=1000000
    bash rsync_da.sh $Da1 $lambda $gamdot $dt $Kc


    
fi
