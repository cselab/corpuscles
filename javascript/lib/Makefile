include ../../conf.mk
M = off.js he.js

install: $M; co.install $M "$(PREFIX)/include"
test:; atest test/*
clean:
lint: $M; jshint --reporter=unix --verbose -c data/conf.json  $M
.PHONY: clean install lint test
