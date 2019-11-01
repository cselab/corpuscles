# i=/u/co/img/dupier.off
#i=/u/.co/rbc/laplace/0.off
i=data/edg.off
./vtk < $i > b.vtk
./a.py b.vtk
./b.py b.vtk
convert a.png[400x400] img/0.png
convert b.png[400x400] img/1.png
./main2.py
convert -background '#A0A0A0' -composite -flatten  o.png  img/1.png q.png
