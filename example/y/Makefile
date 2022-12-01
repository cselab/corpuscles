.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk

M = \
2\
2b\
c\
merge\

PREC = d
CO_FLAGS = `co.conf --cflags --libs $(PREC)`
all: $M
.c:; $(CC) $< $(CFLAGS) $(LDFLAGS) $(CO_FLAGS) -o $@
clean:; rm -f $M
