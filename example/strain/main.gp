s(x) = column(x)/3

plot [-1:2][-1:2]\
     "q" u 'x':'y'           w p pt 7 ps 2 t "", \
     ""  u 'x':'y':(s('fx')):(s('fy')) w vectors t "f", \
     ""  u 'x':'y':(s('hx')):(s('hy')) w vectors t "h"
