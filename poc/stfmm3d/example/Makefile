.POSIX:
M = main
CO_CFLAGS = 
CO_LDFLAGS = -L../lib -lstfmm3d -lgfortran -lm
LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS) $(CO_LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(CO_CFLAGS) $< -c

.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M
install:

.PHONY: clean test lint
clean:; rm -f $M $(M:=.o)
