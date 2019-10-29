.POSIX:
include ../../conf.mk
M = \
grid\
point\
vtk\

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
run: $M; @co.run ./main < `co.path`/rbc/icosahedron/0.off
lint:; make CFLAGS='-Wall -Wextra'

.PHONY: clean test install run lint run
clean:; rm -f $M $(M:=.o)
