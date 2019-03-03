M = main
PREC = s
HE_CFLAGS = -Wall -Wextra -O2 -g `co.conf --cflags $(PREC)`
HE_LDFLAGS = `co.conf --libs $(PREC)`

GSL_CFLAGS  = `gsl-config --cflags`
GSL_LDFLAGS = `gsl-config --libs`

$M: $M.o
	$(CC) $< $(LDFLAGS) $(HE_LDFLAGS) $(GSL_LDFLAGS) -o $@
$M.o: $M.c
	$(CC) $(CFLAGS) $(HE_CFLAGS) $(GSL_CFLAGS) $< -c -o $@

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
