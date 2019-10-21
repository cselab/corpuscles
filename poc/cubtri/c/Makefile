.POSIX:
M = \
main\

LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS) -L../lib -lcubtri -lgfortran -o $@

.c.o:
	$(CC) $(CFLAGS) $(CO_CFLAGS) $< -c

.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M
install:

.PHONY: clean test lint
clean:; rm -f $M $(M:=.o)
