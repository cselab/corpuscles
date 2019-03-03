$ e="0 0 0  1 0 0  0 1 0" a="0 0 0" b="1 0 0" c="0.1 1 0"
$ ./main 2d  $e $a $b $c
-1 -1.1 0
 1  0.1 0
 0  1   0

$ co.shake ./main energy  $e %0 0 0 $b $c
-1

$ co.shake ./main energy  $e 0 %0 0 $b $c
-0.9

$ co.shake ./main energy  $e $a  %1 0 0 $c
1

$ co.shake ./main energy  $e $a  1 %0 0 $c
-0.1

-1 -0.9
1  -0.1
0     1
