.POSIX:
.SUFFIXES: .c .f .f90 .o

CC = c99
FC = gfortran
FCFLAGS = -Og  -g
CFLAGS = -Og  -g

L = libcubtri.a

O = \
main.o\
lib.o\
fun.o\

libcubtri.a: $O ; ar r $@ $O && ranlib $@

.f.o:; $(FC) $(FCFLAGS) $< -c
.f90.o:; $(FC) $(FCFLAGS) $< -c
.c.o:; $(CC) $(CFLAGS) $< -c

.PHONY: clean
clean:; rm -f $O libcubtri.a mod.mod
