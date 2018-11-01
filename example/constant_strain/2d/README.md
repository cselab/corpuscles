$ ./main all 0 0 1 0 0 1      0 0 0 0 0 0
fx fy hx hy
-2 -2 -2 -2
2 -0 2 0
-0 2 0 2

$ he.shake ./main denergy 0 0 1 0 0 1      %0 0 0 0 0 0
2

$ ./main denergy 0 0 1 0 0 1      1e-6 0 0 0 0 0
2.000001e-06

$ ./main denergy 0 0 1 0 0 1      -1e-6 0 0 0 0 0
-1.999999e-06
