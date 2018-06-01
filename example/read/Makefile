M = main
HE_CFLGAS = -Wall -Wextra -O0 -g `he.conf --cflags`
TRI_LDFLAGS = `he.conf --libs`

$M: $M.o; $(CC) $< $(LDFLAGS) $(TRI_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLGAS) $(HE_CFLGAS) $< -c -o $@

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
