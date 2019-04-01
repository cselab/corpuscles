.POSIX:
include ../../conf.mk
M = predicates.o
LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS)  -o $@

.c.o:
	$(CC) $(CFLAGS)  $< -c -o $@

.c:
%: %.c

test: $M; atest test/*
install:
run: $M; @co.run ./morris > q
lint:; make CFLAGS='-Wall -Wextra'

.PHONY: clean test install run lint run
clean:; rm -f $M $(M:=.o)
