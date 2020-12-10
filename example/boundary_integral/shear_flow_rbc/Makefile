.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../../conf.mk
M = main
PREC = d
CO_FLAGS = `co.conf --libs --cflags $(PREC)`
all: $M
.c:; $(FC) $< $(CFLAGS) $(CO_FLAGS) -o $@
lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M; atest test/*
install:
clean:; rm -f $M $(M:=.o)
