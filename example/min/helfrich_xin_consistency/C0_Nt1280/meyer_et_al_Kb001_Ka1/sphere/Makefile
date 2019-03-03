include ../../../../../../conf.mk
M = main voronoi
PREC = d
CO_CFLAGS = -O0 -g `alg.conf --cflags $(PREC)`
CO_LDFLAGS = `alg.conf --libs $(PREC)`

all: $M

main: main.o; $(CC) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@
main.o: main.c; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

voronoi: voronoi.o; $(CC) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@
voronoi.o: voronoi.c; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
