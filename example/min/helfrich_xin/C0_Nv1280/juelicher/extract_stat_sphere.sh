if test $# -ne 0

then
    Vr="$1"
    awk '{ if ($1=="eng:") {print $2, $3, $4}}' sphere_Vr$Vr.msg > sphere_Vr$Vr.eng
    awk '{ if ($3=="Vr:") {print  $4, $5, $6}}' sphere_Vr$Vr.msg > sphere_Vr$Vr.vol
else
    for Vr in 0.95 0.9 0.85 0.8 0.75 0.7 0.65 0.6 0.55 0.5 0.45 0.4 0.35 0.3 0.25 0.2
    do sh extract_stat_sphere.sh $Vr
    done
fi
