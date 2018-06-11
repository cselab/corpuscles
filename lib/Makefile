PREFIX = $(HOME)
HE_CFLAGS = -Wall -Wextra -O2 -g
GSL_CFLAGS =

O_S = off.s.o x.s.o vec.s.o tri.s.o dtri.s.o punto.s.o ten.s.o
O_D = off.d.o x.d.o vec.d.o tri.d.o dtri.d.o punto.d.o ten.d.o
O = edg.o err.o memory.o util.o read.o he.o
H = he/edg.h he/err.h he/dtri.h he/memory.h he/punto.h he/vec.h he/macro.h he/util.h \
    he/ten.h he/read.h he/off.h he/tri.h he/he.h he/x.h he/prec/s/real.h he/prec/d/real.h
I = inc/def.h
TRG = he
L = libhe_s.a libhe_d.a

include make/rules.mk
