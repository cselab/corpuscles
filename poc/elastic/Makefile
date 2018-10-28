P = ../..
include $P/conf.mk

M = linear_strain_tri

PREC = s
HE_CFLAGS =  `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@
$M.o: $M.inc $M.h

$M.inc $M.h: param var body
	mkdir -p he && \
	./function $M param var body

param var body: main.mac
	u.maxima $<

install: $M.o
	cp    linear_strain_tri.inc $P/lib && \
	cp    linear_strain_tri.c   $P/lib && \
	cp he/linear_strain_tri.h   $P/lib/he

.PHONY: clean install

clean:; rm -f param var body $M.inc $M.h main.o
