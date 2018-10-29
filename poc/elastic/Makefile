P = ../..
include $P/conf.mk

M = constant_strain/2d

PREC = s
HE_CFLAGS =  `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) -I. $< -c -o $@
$M.o: $M.inc he/$M.h

$M.inc $M.h: param var body
	mkdir -p constant_strain he/constant_strain && \
	./function constant_strain_2d $M param var body

param var body: main.mac
	u.maxima $<

install: $M.o
	cp    constant_strain_2d.inc $P/lib && \
	cp    constant_strain_2d.c   $P/lib && \
	cp he/constant_strain_2d.h   $P/lib/he

.PHONY: clean install

clean:; rm -f param var body $M.inc $M.h main.o
