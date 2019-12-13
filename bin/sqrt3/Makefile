.POSIX:
.PHONY: clean test install run lint
.SUFFIXES: .o .c

include ../../conf.mk
M = \
main\

PREC = d
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

LINK = $(CC)

install: $M
	mkdir -p $(PREFIX)/bin
	cp main $(PREFIX)/bin/co.sqrt3
.o:
	$(LINK) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M; atest test/*
install:
run: $M; @co.run ./rbc


clean:; rm -f $M $(M:=.o)
