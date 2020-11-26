.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk
M = \
big\
join3\
one\
split3\
tri\

PREC = d
CO_FLAGS = `co.conf --cflags --libs $(PREC)`

all: $M
.c:; $(CC) $< $(CFLAGS) $(CO_FLAGS) -o $@
clean:; rm -f $M $(M:=.o)
