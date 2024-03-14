.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk
M = main
PREC = d
CO_FLAGS = `co.conf --libs --cflags $(PREC)`
all: $M
install: $M
	mkdir -p $(PREFIX)/bin
	cp main $(PREFIX)/bin/co.2xdmf
.c:; $(CC) $< $(CFLAGS) $(CO_FLAGS) -o $@
clean:; rm -f $M
test:
