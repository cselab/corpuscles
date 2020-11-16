M = main
PREC = d
ALG_CFLGAS = `alg.conf --cflags $(PREC)`
ALG_LDFLAGS = `alg.conf --libs $(PREC)`

LINK = $(CC)
all:  $M
.c.o:
	$(CC) $(CFLAGS) $(ALG_CFLGAS) $< -c -o $@
.o:
	$(LINK) $<$(LDFLAGS)  $(ALG_LDFLAGS) -o $@
.c:
%: %.c

test: $M; atest test/*
install:
lint:; make CFLAGS='-Wall -Wextra -g -O2'

.PHONY: clean test install lint
clean:; rm -f $M.o $M
