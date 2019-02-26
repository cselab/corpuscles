include ../../conf.mk
PREC = d
HE_CFLAGS = `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

main: main.c
	$(CC) main.c $(CFLAGS) $(HE_CFLAGS) $(LDFLAGS) $(HE_LDFLAGS) -o main
test:
install:

.PHONY: clean test install
clean:; rm -f main
