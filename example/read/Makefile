M = main
TRI_CFLAGS = -Wall -Wextra -O0 -g -D_FORTIFY_SOURCE=2 `he.conf --cflags`
TRI_LDFLAGS = `he.conf --libs`

$M: $M.o; $(CC) $< $(LDFLAGS) $(TRI_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLGAS) $(TRI_CFLAGS) $< -c -o $@

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
