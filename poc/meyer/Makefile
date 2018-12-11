M = meyer juelicher

all: $M

%: %.m4
	m4 -Im4 $< > $@
	chmod +x $@

run: juelicher; ./juelicher < data/hex.off

.PHONY: clean run

clean:; rm -f $M
