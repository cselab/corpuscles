function err = off_write(f, p, t)
  nv = size(p, 1);
  nt = size(t, 1);
  fprintf(f, "OFF\n");
  fprintf(f, "%d %d 0\n", nv, nt);

  for i = 1:nv
    fprintf(f, "%.16g %.16g %.16g\n", p(i, :));
  endfor

  for i = 1:nt
    fprintf(f, "3 %d %d %d\n", t(i, :) - 1);
  endfor

endfunction
