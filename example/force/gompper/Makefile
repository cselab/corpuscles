M = main
PREC = s
TRI_CFLAGS = -Wall -Wextra -O0 -g `he.conf --cflags $(PREC)`
TRI_LDFLAGS = `he.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(TRI_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLGAS) $(TRI_CFLAGS) $< -c -o $@
$M.o: inc/main.h  inc/mesh.h  inc/uti.h  inc/util.h

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
