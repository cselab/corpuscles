.POSIX:
.SUFFIXES:
.SUFFIXES: .s.o .d.o .l.o .o .f .f90 .c

include ../conf.mk
PREC = d
LINK = $(CC)

CO_S_CFLAGS = `co.conf --cflags s`
CO_D_CFLAGS = `co.conf --cflags d`
CO_L_CFLAGS = `co.conf --cflags l`

L = libfm_s.a libfm_d.a libfm_l.a
H = fm.h cubtri.h

O_D = fm.d.o cubtri_c.d.o
O_S = fm.s.o cubtri_c.s.o
O_L = fm.l.o cubtri_c.l.o

O =\
cubtri_lib.o\
cubtri.o\
main.o\
d3tstrcr.o\
l3dterms.o\
l3dtrans.o\
d3mtreeplot.o\
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

all: $L
libfm_s.a: $O $(O_S); ar r $@ $O $(O_S) && ranlib $@
libfm_d.a: $O $(O_D); ar r $@ $O $(O_D) && ranlib $@
libfm_l.a: $O $(O_L); ar r $@ $O $(O_L) && ranlib $@

.f.o:; $(FC) $(FCFLAGS) $(FXFLAGS) $< -c
.f90.o:; $(FC) $(FCFLAGS) $(FXFLAGS) $< -c
.c.o:; $(CC) $(CFLAGS) $< -c

.c.s.o:; $(CC) $(CFLAGS) $(CO_S_CFLAGS) -c -o $@ $<
.c.d.o:; $(CC) $(CFLAGS) $(CO_D_CFLAGS) -c -o $@ $<
.c.l.o:; $(CC) $(CFLAGS) $(CO_L_CFLAGS) -c -o $@ $<

lint:; $(MAKE) FCFLAGS='-Wall -Wextra -Og -g' CFLAGS='-Wall -Wextra -Og -g'

install: lib hdr
lib: $L
	co.install $L $(PREFIX)/lib
hdr: $H
	co.install $H $(PREFIX)/include

clean:; rm -f -- $O $L
