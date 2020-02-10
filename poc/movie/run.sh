export COLORMAP=autumn
export WY=800
export WY=800

co.geomview -O -n none -t 0 0 -10 -r 90 0 0 -a cell_app_yes -f 14 -p '(co.transform -c | co.sqrt3 | co.sqrt3 | co.list -n
      "(co.xcolor -o 0.7 -t -n ref/ref_rotate_ss.off 0.8 1 )"
      "(co.bead -b bead.off -r ref/ref_rotate_ss.off | co.app bead_app_red)" 
      "(co.bead -y -b bead.off -r ref/ref_rotate_ss.off | co.app bead_app_yellow)" ) <' Da0.143_lam1.0000_g0.0288_dt0.005_Kc20000_cont/*000000.off
