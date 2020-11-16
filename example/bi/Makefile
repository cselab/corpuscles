.POSIX:
include ../../conf.mk
PREC = d
M = \
double\
err\
f2v\
pointer\
r2v\
single\
v2v\
velocity\
velocity_double\
volume\

CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`
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
