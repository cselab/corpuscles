M = main
PREC = s
HE_CFLAGS = -Wall -Wextra -O2 -g `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

GSL_CFLAGS  = `gsl-config --cflags`
GSL_LDFLAGS = `gsl-config --libs`

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) $(GSL_LDFLAGS) -o $@
$M.o: $M.c
	$(CC) $(CFLAGS) -Iinc/paper $(HE_CFLAGS) $(GSL_CFLAGS) $< -c -o $@

$M.o: inc/far/sys.h inc/lor/sys.h inc/wolf/lor/sys.h \
	inc/wolf/far/sys.h inc/paper/sys.h

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
