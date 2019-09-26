.POSIX:
include ../../../conf.mk
PREC = d
M = \
main\
single\
double\
oseen\

CO_CFLAGS = -I../lib `co.conf --cflags $(PREC)`
CO_LDFLAGS = -L../lib -lstfmm3d `co.conf --libs $(PREC)` -lgfortran -lm 
LINK = $(CC)

all: $M
.o:
	$(LINK) $< $(LDFLAGS) $(CO_LDFLAGS) -o $@


.c.o:
	$(CC) $(CFLAGS) $(CO_CFLAGS) $< -c

.c:
%: %.c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
test: $M
install:

.PHONY: clean test lint
clean:; rm -f $M $(M:=.o)
