include ../../conf.mk

M = main
PREC = s
CO_CFLAGS =  `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`
ALG_LDFLAGS = `alg.conf --libs $(PREC)`

install: $M
	mkdir -p $(PREFIX)/bin
	cp main $(PREFIX)/bin/co.eigenvector

$M: $M.o; $(CC) $< $(LDFLAGS) $(CO_LDFLAGS) $(ALG_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

test: install; atest test/*
lint:; $(MAKE) CFLAGS='-Wall -Wextra -Og -g'

.PHONY: clean test install lint
clean:; rm -f $M.o $M
