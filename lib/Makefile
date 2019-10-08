include ../conf.mk

LIB_CFLAGS_S = `alg.conf --cflags s` `fm.conf --cflags s`
LIB_CFLAGS_D = `alg.conf --cflags d` `fm.conf --cflags d`
LIB_CFLAGS_L = `alg.conf --cflags l` `fm.conf --cflags l`

L = lib$P_s.a lib$P_d.a lib$P_l.a

include make/hdr.mk
include make/obj.mk
include make/rules.mk
include make/dep.mk
