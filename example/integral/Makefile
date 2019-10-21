.POSIX:
include ../../conf.mk
M = \
cons\
cons2\
cons2d\
density\
density2d\
line\
line2\
plane\
plane2\
quad\
tri\
visc\
visc2\
visc2d\

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
lint:; make CFLAGS='-Wall -Wextra -O2'

.PHONY: clean test install run lint
clean:; rm -f $M $(M:=.o)
