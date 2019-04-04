.POSIX:
include ../../conf.mk
M = main
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
run: $M; @co.run ./mesh < /u/1.off > q
lint:; make CFLAGS='-Wall -Wextra'

.PHONY: clean test install run lint run
clean:; rm -f $M $(M:=.o)
