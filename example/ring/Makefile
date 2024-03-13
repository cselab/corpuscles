.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk
M = main normal H
PREC = d
CO_FLAGS = `co.conf --libs --cflags $(PREC)`
all: $M
.c:; $(CC) $< $(CFLAGS) $(CO_FLAGS) -o $@
clean:; rm -f $M
