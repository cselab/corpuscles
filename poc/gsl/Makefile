M = main
PREC = s
HE_CFLGAS = -Wall -Wextra -O2 -g `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

GSL_CFLGAS  = `gsl-config --cflags`
GSL_LDFLAGS = `gsl-config --libs`

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) $(GSL_LDFLAGS) -o $@
$M.o: $M.c
	$(CC) $(CFLGAS) -Iinc/wolf $(HE_CFLGAS) $(GSL_CFLGAS) $< -c -o $@

$M.o: inc/far/sys.h inc/lor/sys.h inc/wolf/sys.h
test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
