include ../../conf.mk
PREC = d
CO_CFLAGS = $(co.conf --cflags $(PREC))
CO_LDFLAGS = $(co.conf --libs $(PREC))

main: main.c
	$(CC) main.c $(CFLAGS) $(CO_CFLAGS) $(LDFLAGS) $(CO_LDFLAGS) -o main

.PHONY: clean
clean:; rm -f main
