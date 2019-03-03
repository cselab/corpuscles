include ../../conf.mk
PREC = d
HE_CFLAGS = `co.conf --cflags $(PREC)`
HE_LDFLAGS = `co.conf --libs $(PREC)`

main: main.c
	$(CC) main.c $(CFLAGS) $(HE_CFLAGS) $(LDFLAGS) $(HE_LDFLAGS) -o main
test:
install:

.PHONY: clean test install doc
clean:; rm -f main
