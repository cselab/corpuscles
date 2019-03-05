include(`co.m4')dnl
include(`../m4/util.m4')dnl
r_nav(geomveiw)dnl

# geomview wrapper

```sh
co.geomview -f 25 $(co.path)/regular/octa.off
````


## keys (move with mouse):  
`r`:      rotate (default)  
`t`:      translate  
`z`:      zoom   
`space`:  prints: r, t, z  


## mesh appearance: `-a`  
appearance files are located in corpuscles/bin/geomview/data  
`co.geomview -a app2 <PATH>/*.off`  


## prescribe `r`, `t`, `z`: 
`i="-t 0 0 0 -r 53.5876 -31.0801 -7.53952 -f 24.0571"`  
`co.geomview $i <PATH>/*.off`  


## save image for single off-file: `-o`  
`co.geomview -o image.ppm $i -a app2 <PATH>/file.off>`  


## save image for many off-files: `-O`  
`co.geomview -O $i -a app2 <PATH>/*.off`  


## make movie:  
`co.ffmpeg -o movie.mp4 <PATH>/*.ppm`  


## process ply files directly:  
1. create script ~/bin/gmv  
2. add ~/bin in env variable PATH  
3. copy the following inside ~/bin/gmv: co.geomview -p 'ply2off <' "$@"  
4. use as:  
`gmv -O $i -a app2 <PATH>/*.ply`  

