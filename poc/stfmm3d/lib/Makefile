.POSIX:
CC = c99
FC = gfortran
LINK = $(CC)
FCFLAGS = -g -O2
CFLAGS = -g -O2
LDFLAGS = -lgfortran -lm

M = main
O = \
stfmm3dpart.f

all: $M
main.o: sz.h

$M: $O; $(LINK) $O $(LDFLAGS) -o $@
.f.o:; $(FC) $(FCFLAGS) $< -c -o $@
.c.o:; $(CC) $(CFLAGS) $< -c -o $@

.SUFFIXES: .o .f .c

lint:; $(MAKE) FCFLAGS='-Wall -Wextra -O2 -g'

.PHONY: clean lint
clean:; rm -f -- $O $M
