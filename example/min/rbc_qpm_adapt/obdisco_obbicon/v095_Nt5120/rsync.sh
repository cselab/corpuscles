Kc=1000
freqk=100
coefk=1.1
xi=0.1
dt=0.000001
kBT=0.0

end=0500000


if test $# -ne 0
then
    Da1="$1"

    #create a folder to run and folder name consists of parameters
    if [ ! -d Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1} ]; then
	mkdir Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1}
    fi

    cd Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1}
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/min/rbc_qpm_adapt/obdisco_obbicon/v095_Nt5120/Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1}/${end}.off .
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/min/rbc_qpm_adapt/obdisco_obbicon/v095_Nt5120/Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1}/stat.dat .
    rsync -avz euler:/cluster/scratch/bianx/run/corpuscles/example/min/rbc_qpm_adapt/obdisco_obbicon/v095_Nt5120/Kc${Kc}_freqk${freqk}_coefk${coefk}_Da${Da1}/lsf* .
    cd ..
    
else

    Da1=0.143
    bash rsync.sh $Da1

    Da1=-0.858
    bash rsync.sh $Da1

    Da1=-0.358
    bash rsync.sh $Da1

    Da1=0.072
    bash rsync.sh $Da1

    Da1=1.717
    bash rsync.sh $Da1

    Da1=1.788
    bash rsync.sh $Da1

    Da1=2.003
    bash rsync.sh $Da1

fi
