real.s.o: inc/s/real.inc
real.d.o: inc/d/real.inc

constant_strain/2d.s.o: constant_strain/2d.inc

KERNEL = \
kernel/gauss.inc\
kernel/quintic.inc\
kernel/wendland6.inc\
kernel/yang.inc\
kernal/main.inc

kernal.s.o kernal.d.o kernal.l.o: $KERNEL
predicate/main.o: predicate/main.inc