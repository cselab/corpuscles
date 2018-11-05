echo "#Vr Ebend" > oblate_final.dat
for Vr in 0.95 0.9 0.85 0.8 0.75 0.7 0.65 0.6 0.55 0.5 0.45 0.4 0.35 0.3 0.25 0.2
do
    paste oblate_Vr$Vr.eng oblate_Vr$Vr.vol > temp1.dat
    tail -n 1 temp1.dat > temp2.dat
    awk '{print $6, $2}' temp2.dat >> oblate_final.dat
    rm temp1.dat
    rm temp2.dat
done
