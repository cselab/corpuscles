.POSIX:
include ../../conf.mk
PREC = d
M = \
err\
single\
f2v\
v2v\
r2v\
volume\
double\

CO_CFLAGS = `fm.conf --cflags $(PREC)`
CO_LDFLAGS = `fm.conf --libs $(PREC)` `alg.conf --libs $(PREC)` `co.conf --libs $(PREC)` `fm.conf --libs $(PREC)`
LINK = $(CC)

all: $M
.o:; $(LINK) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@
.c.o:; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c
.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M; atest test/*
install:

.PHONY: clean test lint
clean:; rm -f $M $(M:=.o)
