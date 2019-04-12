.POSIX:
include ../../../conf.mk
M = cell cons density morris mesh pre
PREC = d
CO_CFLAGS = `alg.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)` `alg.conf --libs $(PREC)` 

LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

.c:
%: %.c

test: $M; atest test/*
install:
run: $M; @co.run ./density < /u/1.off > q
lint:; make CFLAGS='-Wall -Wextra -O2 -g'

.PHONY: clean test install run lint run
clean:; rm -f $M $(M:=.o)
