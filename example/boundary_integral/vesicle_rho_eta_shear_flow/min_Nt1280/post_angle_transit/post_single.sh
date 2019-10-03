for i
do
    eigenvector < "$i" | awk '{if ($3<0) {$1=-$1; $3=-$3;} print atan2($3, $1), atan2($3, $1)/3.1415926}'
done
