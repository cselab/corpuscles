export COLORMAP=autumn
export WY=800
export WY=800

ref=ref/ref_rotate_sss.off
co.sqrt3 < ref/ref_rotate_ss.off > $ref

co.geomview -n none -t 0 0 -10 -r 90 0 0 -a cell_app_yes -f 14 -p \
            "(co.transform -c | co.sqrt3 | co.sqrt3 | co.sqrt3 | co.list -n
      \"(co.xcolor -n -t -o 0.8 -s 0.9 -f 4 -t $ref -1 1)\"
      \"(co.bead    -b bead.off -r $ref | co.app bead_app_red)\" 
      \"(co.bead -y -b bead.off -r $ref | co.app bead_app_yellow)\" ) <" "$@"
