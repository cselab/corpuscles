.POSIX:
.SUFFIXES: .c .o
.PHONY: all lint clean

M_LDFLAGS = `pkg-config --libs libtiff-4`
M_CFLAGS = `pkg-config --cflags libtiff-4`
CFLAGS = -Og -g
LINK = $(CC)
M = \
main\

all: $M
.o:; $(LINK) $< $(M_LDFLAGS) $(LDFLAGS) -o $@
.c.o:; $(CC) $(M_CFLAGS) $(CFLAGS) $< -c

.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -Og'
clean:; rm -f $M $(M:=.o) $(M:=.c)
