Da1=0.143

dt0=0.01

gamdot=7.1961916509
dt=0.000004
scale=2
dt=$(echo $dt $scale | awk '{print $1*$2}')
Kc=1000000

#bash run.sh $Da1 $gamdot $dt $Kc


gamdot=0.021588574953
dt=0.00067
scale=10
dt=$(echo $dt $scale | awk '{print $1*$2}')
Kc=20000

bash run.sh $Da1 $gamdot $dt $Kc
