set -eu

Kc=20000

if test $# -ne 0
then

    Da1="$1"
    gamdot="$2"
    dt="$3"
    
    gam=$(printf "%.4f" $gamdot)
    echo "gam="$gam
    if [ ! -d Da${Da1}_g${gam}_dt${dt}_Kc${Kc} ]; then
	
	mkdir Da${Da1}_g${gam}_dt${dt}_Kc${Kc}
	
    fi
    
    cd Da${Da1}_g${gam}_dt${dt}_Kc${Kc}

    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_shear_flow/ade_Nt1280/Da${Da1}_g${gam}_dt${dt}_Kc${Kc}/*.off .
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_shear_flow/ade_Nt1280/Da${Da1}_g${gam}_dt${dt}_Kc${Kc}/msg.out .
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_shear_flow/ade_Nt1280/Da${Da1}_g${gam}_dt${dt}_Kc${Kc}/stat.dat .
    #rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/boundary_integral/sde_rho_shear_flow/ade_Nt1280/Da${Da1}_g${gam}_dt${dt}_Kc${Kc}/lsf* .

    
else

    Da1=0.143

    dt=0.1
    
    gamdot=0.00143923833018
    bash rsync_da.sh $Da1 $gamdot $dt

    gamdot=0.00287847666036
    bash rsync_da.sh $Da1 $gamdot $dt

    gamdot=0.0071961916509
    bash rsync_da.sh $Da1 $gamdot $dt

    dt=0.01
    
    gamdot=0.0143923833018
    bash rsync_da.sh $Da1 $gamdot $dt

    gamdot=0.0287847666036
    bash rsync_da.sh $Da1 $gamdot $dt

    gamdot=0.071961916509
    bash rsync_da.sh $Da1 $gamdot $dt

    dt=0.001

    gamdot=0.143923833018
    bash rsync_da.sh $Da1 $gamdot $dt

    gamdot=0.287847666036
    bash rsync_da.sh $Da1 $gamdot $dt

    gamdot=0.71961916509
    bash rsync_da.sh $Da1 $gamdot $dt

    dt=0.0001

    gamdot=1.43923833018
    bash rsync_da.sh $Da1 $gamdot $dt
    
    gamdot=2.87847666036
    bash rsync_da.sh $Da1 $gamdot $dt

    gamdot=7.1961916509
    bash rsync_da.sh $Da1 $gamdot $dt


    
fi
