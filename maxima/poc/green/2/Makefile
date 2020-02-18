.POSIX:
include ../../../../conf.mk
M = \
main\

PREC = d
CO_CFLAGS = \
`co.conf --cflags $(PREC)`\
`alg.conf --cflags $(PREC)`\

CO_LDFLAGS = \
`co.conf --libs $(PREC)`\
`alg.conf --libs $(PREC)`\
`co.conf --libs $(PREC)`\

LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: all; atest test/*

.PHONY: clean lint test
clean:; rm -f $M $(M:=.o)
