include ../../conf.mk
PREC = d
main: main.c
	$(CC) main.c $(CFLAGS)  `co.conf --cflags --libs $(PREC) ` $(LDFLAGS) -o $@
clean:; rm -f main
