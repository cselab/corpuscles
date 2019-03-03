Run 

    poc/xin/geomview poc/xin/list.dat
	co.m4 poc/xin/gnuplot | gnuplot - && feh q.png


Sergey

    poc/xin/sim | awk '$2 == "bc" && $4 == 5120 && $5 == "B"' | poc/xin/prefer | awk -v OFS="\t" '{print $6, $7, $NF}' | sort -g > q


