.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk
M = read relax sph write vtk
PREC = d
CO_FLAGS = `co.conf --libs --cflags $(PREC)`
all: $M
.c:; $(CC) $< $(CFLAGS) $(CO_FLAGS) -o $@
clean:; rm -f $M
