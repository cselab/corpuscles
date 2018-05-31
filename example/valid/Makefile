M = main
PREC = d
TRI_CFLAGS = -Wall -Wextra -O0 -g `he.conf --cflags $(PREC)`
TRI_LDFLAGS = `he.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(TRI_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLGAS) $(TRI_CFLAGS) $< -c -o $@

test: $M
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
