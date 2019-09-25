.POSIX:
CC = c99
FC = gfortran
LINK = $(CC)
FCFLAGS = -g -O2
CFLAGS = -g -O2
LDFLAGS = -lgfortran -lm

O = \
main.o\
stfmm3dpart.o

libstfmm3d.a: $O; ar r $@ $O && ranlib $@

$M: $O; $(LINK) $O $(LDFLAGS)
.f.o:; $(FC) $(FCFLAGS) $< -c
.f90.o:; $(FC) -std=f95 $(FCFLAGS) $< -c
.c.o:; $(CC) $(CFLAGS) $< -c

.SUFFIXES: .o .f .f90 .c

lint:; $(MAKE) FCFLAGS='-Wall -Wextra -O2 -g'

.PHONY: clean lint
clean:; rm -f -- $O $M
