.POSIX:
include ../../conf.mk
M = \
main\

PREC = d
CO_CFLAGS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

LINK = $(CC)

install:
.o:
	$(LINK) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(CO_CFLAGS) $< -c -o $@

.c:
%: %.c
%: %.sh

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M; atest test/*
install: $M
	mkdir -p $(PREFIX)/bin
	cp main $(PREFIX)/bin/co.transform

.PHONY: clean test install run lint
clean:; rm -f $M $(M:=.o)
