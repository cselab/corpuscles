.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk

M = main
PREC = d
CO_FLAGS = `co.conf --cflags --libs $(PREC)`

all: $M
.c:; $(CC) $< $(LDFLAGS) $(CFLAGS) $(CO_FLAGS) -o $@
install:
clean:; rm -f $M.o $M
