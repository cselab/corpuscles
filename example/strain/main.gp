s(x) = column(x)/3

plot [-1:2][-1:2]\
     "q" u 'x':'y'             w p pt 7 ps 2   t "", \
     ""  u 'x0':'y0'           w p pt 7 ps 1.4 t "", \
     ""  u 'x':'y':(-s('fx')):(-s('fy')) w vectors t "f"
