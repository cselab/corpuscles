	poc/xin/sim | awk '$2 == "bc" && $4 == 5120 && $5 == "B"' | poc/xin/prefer | awk -v OFS="\t" '{print $6, $7, $NF}' | sort -g > q


	plot [0:1000][0:1000] 'img/kantor.png' binary filetype=png center=(500,500) dx=0.2 dy=0.2 with rgbimage notitle

    convert -transparent "rgb(255,255,255)" kantor.png q.png


Run 

    poc/xin/geomview poc/xin/list.dat
	he.m4 poc/xin/gnuplot | gnuplot - && feh q.png
