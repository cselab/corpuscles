P = ../..
include $P/conf.mk

M = constant_strain_2d

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
	cp    constant_strain_2d.inc $P/lib && \
	cp    constant_strain_2d.c   $P/lib && \
	cp he/constant_strain_2d.h   $P/lib/he

.PHONY: clean install

clean:; rm -f param var body $M.inc $M.h main.o
