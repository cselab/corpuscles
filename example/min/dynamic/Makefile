include ../../../conf.mk
M = main
PREC = d
HE_CFLAGS =  `he.conf --cflags $(PREC)`
HE_LDFLAGS = `he.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@

test: $M; atest test/*
install:

.gp.pdf: 
	gnuplot $< && \
        pdfcrop --margin 10 $@ /tmp/t.pdf && \
	mv /tmp/t.pdf $@

.PHONY: clean test install pdf
clean:; rm -f $M.o $M
