if test $# -ne 0
then
    Vr="$1"
    convert kantor_nelson_C0_prolate_Vr$Vr.ppm -resize 420  kantor_nelson_C0_prolate_Vr$Vr.png
    convert kantor_nelson_C0_oblate_Vr$Vr.ppm -resize 420  kantor_nelson_C0_oblate_Vr$Vr.png
    convert kantor_nelson_C0_sphere_Vr$Vr.ppm -resize 420  kantor_nelson_C0_sphere_Vr$Vr.png
else
    for Vr in 0.95 0.9 0.85 0.8 0.75 0.7 0.65 0.6 0.55 0.5
    do sh resize_image.sh $Vr
    done
fi
