export COLORMAP=autumn
export WY=800
export WY=800

ref=ref/ref_rotate_sss.off
co.sqrt3 < ref/ref_rotate_ss.off > $ref

co.geomview -a none -n none -t 0 0 -10 -r 90 0 0 -a cell_app_yes -f 14 -p "(co.transform -c | co.sqrt3 | co.sqrt3 | co.sqrt3 | co.list -n
      \"(co.xcolor -o 0.8 -t -n $ref 0.73 0.83 )\"
      \"(co.bead -b bead.off -r $ref | co.app bead_app_red)\" 
      \"(co.bead -y -b bead.off -r $ref | co.app bead_app_yellow)\" ) <" "$@"
