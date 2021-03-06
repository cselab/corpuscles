#ref="../ref/ref_rotate.off"
ref="../ref/ref_rotate_r.off"

sa=0.0
sb=1.0

wx1=600
wx2=800
wy1=600
wy2=800

fos=33
fot=33

yes=1
if [ $yes -eq 1 ]; then
    file_in="../ref/ref_v0.95_r.off"
    file_out="ref"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../rest/rest_r.off"
    file_out="rest"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app2a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../TT1_A/num40_r.off"
    file_out="tt1a_n40"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app2a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../TT1_B/num20_r.off"
    file_out="tt1b_n20"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app2a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../TT1_B/num40_r.off"
    file_out="tt1b_n40"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app2a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../TT2_A/num60_r.off"
    file_out="tt2a_n60"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app2a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../TT2_A/num80_r.off"
    file_out="tt2a_n80"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app2a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../TT2_B/num60_r.off"
    file_out="tt2b_n60"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app2a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../TT2_B/num80_r.off"
    file_out="tt2b_n80"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app2a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi

yes=1
if [ $yes -eq 1 ]; then
    file_in="../TT3_B/num200_r.off"
    file_out="tt3b_n200"
    WX=$wx2 WY=$wy2 co.geomview  -r 90 0 0 -f $fos -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_s.ppm $file_in
    WX=$wx2 WY=$wy2 co.geomview  -r 0 0 0 -f $fot -a ~/app/app1a -p "co.xcolor -a $ref $sa $sb" -o ${file_out}_t.ppm $file_in
    convert ${file_out}_s.ppm ${file_out}_s.png
    convert ${file_out}_t.ppm ${file_out}_t.png
    rm ${file_out}_s.ppm
    rm ${file_out}_t.ppm
fi
