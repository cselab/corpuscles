.POSIX:
include ../../conf.mk
M = main
GTS_CFLAGS = `pkg-config --cflags gts`
GTS_LDFLAGS = `pkg-config --libs gts` -Wl,-R`pkg-config  gts  --variable=libdir`

LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS) $(GTS_LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(GTS_CFLAGS) $< -c -o $@

.c:
%: %.c

test: $M; atest test/*
install:
run: $M; @co.run ./main

.PHONY: clean test install run
clean:; rm -f $M $(M:=.o)
