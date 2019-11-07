include ../../conf.mk
M = \
a\
c1\
c2\
c3\
i\
ini\

O = $(M:=.o)
PREC = d
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

all: $M
%: %.c
.o:; $(CC) $^ $(LDFLAGS) $(CO_LDFLAGS) -o $@
.c.o:; $(CC) $(CFLAGS) $(CO_CFLAGS) $^ -c -o $@

test: $M; atest test/*
install:

.PHONY: clean test install all
.SUFFIXES: .o .c
clean:; rm -f  $M $O
