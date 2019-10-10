.POSIX:
include ../../conf.mk
PREC = d
M = \
single\
f2v\
v2v\
double\

CO_CFLAGS = `fm.conf --cflags $(PREC)`
CO_LDFLAGS = `fm.conf --libs $(PREC)` `co.conf --libs $(PREC)` `fm.conf --libs $(PREC)`
LINK = $(CC)

all: $M
.o:; $(LINK) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@
.c.o:; $(CC) $(CFLAGS) $(CO_CFLAGS) $< -c
.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M
install:

.PHONY: clean test lint
clean:; rm -f $M $(M:=.o)
