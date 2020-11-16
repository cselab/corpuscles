.POSIX:
M = vectors values
PREC = d
ALG_CFLAGS = `alg.conf --cflags $(PREC)`
ALG_LDFLAGS = `alg.conf --libs $(PREC)`

LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS) $(ALG_LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(ALG_CFLAGS) $< -c -o $@

.c:
%: %.c

test: $M; atest test/*
install:
run: $M; @co.run ./main

.PHONY: clean test install run
clean:; rm -f $M $(M:=.o)
