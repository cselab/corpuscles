include ../../conf.mk

M = linear_strain_tri

PREC = s
HE_CFLAGS =  `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@
$M.o: $M.inc $M.h

$M.inc $M.h: param var body
	./function $M param var body

param var body: main.mac
	u.maxima $<

.PHONY: clean

clean:; rm -f param var body $M.inc $M.h main.o
