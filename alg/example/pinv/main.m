#!/usr/bin/env octave

f = stdin();
dim = fscanf(f, "%d", 1);
m = dlmread(f);
printf("%d\n", dim);
dlmwrite(stdout(), pinv(m), "precision", "%.16g", "delimiter", " ");
