.POSIX:
.SUFFIXES:
.SUFFIXES: .c
include ../../conf.mk
M = main
PREC = d
CO_FLAGS = `co.conf --cflags --libs $(PREC)`
install: $M
	mkdir -p $(PREFIX)/bin
	cp main $(PREFIX)/bin/co.ply2off

.c:; $(CC) $< $(CFLAGS) $(LDFLAGS) $(CO_FLAGS) -o $@
install:
clean:; rm -f $M $(M:=.o)
