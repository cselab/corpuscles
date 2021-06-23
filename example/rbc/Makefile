.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk
M = \
double\
main\
velocity\

PREC = d
CO_FLAGS = `co.conf --libs --cflags $(PREC)`
LINK = $(FC)

all: $M
.c:; $(LINK) $(CFLAGS) $< -o $@ $(CO_FLAGS) $(LDFLAGS)

clean:; rm -f $M $(M:=.o)
