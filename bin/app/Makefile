include ../../conf.mk

M = main
install: $M
	mkdir -p $(PREFIX)/bin
	cp main $(PREFIX)/bin/co.app

test: install; atest test/*

.PHONY: clean test install
clean:
