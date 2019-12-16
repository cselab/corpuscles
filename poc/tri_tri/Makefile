.POSIX:
include ../../conf.mk
PREC = d
CO_CFLAGS = `co.conf --cflags $(PREC)`
LINK = $(CC)
L = libtri_tri_$(PREC).a
O = main.o

$L: $O; ar rv $@ $O && ranlib $@
.c.o:; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c

.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M; atest test/*
install:
run: $M; @co.run ./rbc

.PHONY: clean test install run lint
clean:; rm -f $M $(M:=.o)
