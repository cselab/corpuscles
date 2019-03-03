S = ../..
include $S/conf.mk

M = strain/2d

PREC = s
HE_CFLAGS =  `co.conf --cflags $(PREC)`
HE_LDFLAGS = `co.conf --libs $(PREC)`

$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) -I. $< -c -o $@
$M.o: $M.inc he/$M.h

$M.inc $M.h: param var body
	mkdir -p strain he/strain && \
	./function strain_2d $M param var body

param var body: main.mac
	u.maxima $<

install: $M.o
	co.install strain/2d.inc   $P/lib && \
	co.install strain/2d.c     $P/lib && \
	co.install he/strain/2d.h  $P/lib

.PHONY: clean install

clean:; rm -f param var body $M.inc $M.h main.o
