main: main.m4 math.m4
	m4 $< > $@
	chmod +x main

run: main; ./main < data/hex.off
