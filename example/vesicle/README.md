# Visualization

    co.geomview -n none -t 0 0 -2 -r 90 0 0 -p 'co.wall -w -2 < ' *.off

    co.geomview -p '
    co.list -n
    "(co.xcolor 0000000.off 0 1)"
    "(co.bead -x -b /u/b.off -r 0000000.off)"
    <' *00.off
