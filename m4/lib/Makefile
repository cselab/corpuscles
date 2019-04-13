include ../../conf.mk
M = $P.m4 mh.m4

install: $M; co.install $M "$(PREFIX)/include"
test:; atest test/*
clean:
.PHONY: install test clean
