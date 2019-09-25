.POSIX:
.POSIX:
include ../../../conf.mk
PREC = d
FC = gfortran
LINK = $(CC)
FCFLAGS = -g -O2
CFLAGS = -g -O2
LDFLAGS = -lgfortran -lm
CO_CFLAGS = `co.conf --cflags $(PREC)`

O = \
fmm3.o\
main.o\
d3tstrcr.o\
l3dterms.o\
l3dtrans.o\
laprouts3d.o\
legeexps.o\
lfmm3drouts.o\
lfmm3dparthess.o\
lfmm3dparthess_tf.o\
prini.o\
rotviarecur3.o\
st3green.o\
st3routs.o\
stfmm3dpart.o\
yrecursion.o\

libstfmm3d.a: $O; ar r $@ $O && ranlib $@

$M: $O; $(LINK) $O $(LDFLAGS)
.f.o:; $(FC) $(FCFLAGS) $< -c
.f90.o:; $(FC) $(FCFLAGS) $< -c
.c.o:; $(CC) $(CO_CFLAGS) $(CFLAGS) $< -c

.SUFFIXES: .o .f .f90 .c

lint:; $(MAKE) FCFLAGS='-Wall -Wextra -Og -g' CFLAGS='-Wall -Wextra -Og -g'

.PHONY: clean lint
clean:; rm -f -- $O $M
