echo "#et, eb, ea, ega, ev, ek, ee" >  shrink.eng
awk '{ if ($1=="eng:") {print $2, $3, $4, $5, $6, $7, $8}}' shrink.msg >> shrink.eng

echo "#A/A0, V/V0, Vr" > shrink.vol
awk '{ if ($3=="Vr:") {print  $4, $5, $6}}' shrink.msg >> shrink.vol
