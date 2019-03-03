include ../../../conf.mk
M = main
PREC = d
HE_CFLAGS =  `co.conf --cflags $(PREC)`
HE_LDFLAGS = `co.conf --libs $(PREC)`

$M: $M.o; $(CC) $< $(LDFLAGS) $(HE_LDFLAGS) -o $@
$M.o: $M.c; $(CC) $(CFLAGS) $(HE_CFLAGS) $< -c -o $@

test: $M; atest test/*
install:

%.pdf: %.gp
	gnuplot $< && \
        pdfcrop --margin 10 $@ /tmp/t.pdf && \
	mv /tmp/t.pdf $@


pdf: energy.pdf ad.pdf

.PHONY: clean test install pdf
clean:; rm -f $M.o $M
