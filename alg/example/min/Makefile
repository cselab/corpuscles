M = main
PREC = d
ALG_CFLGAS = -Wall -Wextra -O0 -g `alg.conf --cflags $(PREC)`
ALG_LDFLAGS = `alg.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(ALG_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLGAS) $(ALG_CFLGAS) $< -c -o $@

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
