include ../../conf.mk
M = \
area.js\
dat.gui.js\
he.js\
off.js\
three.js\
tri.js\
vec.js\

install: $M; co.install $M "$(PREFIX)/include"
test:; atest test/*
clean:
lint: $M; jshint --reporter=unix --verbose -c data/conf.json  $M
.PHONY: clean install lint test
