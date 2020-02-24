.POSIX:
.PHONY: clean test lint
.SUFFIXES: .o .c

include ../../conf.mk
M = \
main\
inf\

PREC = d
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M; atest test/*
clean:; rm -f $M $(M:=.o)
