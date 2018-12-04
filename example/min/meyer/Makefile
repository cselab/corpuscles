include ../../../conf.mk
M = main
PREC = d
HE_CFLAGS = -O0 -g `alg.conf --cflags $(PREC)`
HE_LDFLAGS = `alg.conf --libs $(PREC)`

all: $M

main: main.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) -o $@
main.o: main.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@

test: $M; atest test/*
install:

.PHONY: clean test install
clean:; rm -f $M.o $M
