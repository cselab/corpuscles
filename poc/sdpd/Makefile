.POSIX:
include ../../conf.mk
M = rho main morris
PREC = d
CO_CFLAGS = `alg.conf --cflags $(PREC)`
CO_LDFLAGS = `alg.conf --libs $(PREC)`

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
run: $M; @co.run ./main

.PHONY: clean test install run
clean:; rm -f $M $(M:=.o)
