M = main
PREC = d
CO_CFLGAS = `co.conf --cflags $(PREC)`
CO_LDFLAGS = `co.conf --libs $(PREC)`

LINK = $(CC)
all:  $M
.c.o:
	$(CC) $(CFLAGS) $(CO_CFLGAS) $< -c -o $@
.o:
	$(LINK) $<$(LDFLAGS)  $(CO_LDFLAGS) -o $@
.c:
%: %.c

test: $M; atest test/*
install:
lint:; make CFLAGS='-Wall -Wextra -g -O2'

.PHONY: clean test install lint
clean:; rm -f $M.o $M
