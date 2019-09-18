function [p, t, err] = off_read(f)
  s = fgetl(f);
  if strcmp(s, "OFF") == 0
    warning("off_read: not an off file");
    err = 1;
    return;
  end
  s = fgetl(f);
  v = sscanf(s, "%d %d %d");
  nv = v(1);
  nt = v(2);
  p = zeros(nv, 3);
  t = zeros(nt, 3);
  for i=1:nv
      s = fgetl(f);
      v = sscanf(s, "%g %g %g");
      p(i, :) = v;
  end
  for i=1:nt
      s = fgetl(f);
      v = sscanf(s, "3 %d %d %d");
      t(i, :) = v + 1;
  end
  err = 0;
end
