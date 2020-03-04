.POSIX:
.SUFFIXES: .c .o
.PHONY: all lint clean

include ../../../conf.mk

PREC = d
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

M_LDFLAGS = `pkg-config --libs libtiff-4`
M_CFLAGS = `pkg-config --cflags libtiff-4`

LINK = $(CC)
M = \
byte\
dir\
imagej\
images\
info\
main\
roi\
vtk\

all: $M
.o:; $(LINK) $< $(M_LDFLAGS) $(CO_LDFLAGS) $(LDFLAGS) -o $@
.c.o:; $(CC) $(M_CFLAGS) $(CO_CFLAGS) $(CFLAGS) $< -c

.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -Og'
clean:; rm -f $M $(M:=.o)
