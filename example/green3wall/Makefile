.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk
M = \
grid\
main\
stress\
stress0\
velocity\
velocity0\
velocity2\

PREC = d
CO_FLAGS = `co.conf --libs --cflags $(PREC)`
all: $M
.c:; $(CC) $< $(CFLAGS) $(CO_FLAGS) -o $@
clean:; rm -f $M
