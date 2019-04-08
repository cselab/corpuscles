.POSIX:
include ../../conf.mk
M = cons plane plane2
PREC = d
CO_CFLAGS = `alg.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`  `alg.conf --libs $(PREC)` 

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
run: $M; @co.run ./plane

.PHONY: clean test install run
clean:; rm -f $M $(M:=.o)
