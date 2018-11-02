$ e="0 0 0  1 0 0  0 1 0" a="0 0 0" b="1 0 0" c="0.1 1 0"
$ ./main force  $e $a $b $c
-1 -1.1 0
 1  0.1 0
 0  1   0

$ he.shake ./main energy  $e %0 0 0 $b $c
-1

$ he.shake ./main energy  $e 0 %0 0 $b $c
-0.9

$ he.shake ./main energy  $e $a  %1 0 0 $c
1

$ he.shake ./main energy  $e $a  1 %0 0 $c
-0.1

-1 -0.9
1  -0.1
0     1
