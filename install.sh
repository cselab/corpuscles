#!/bin/sh

(cd tool && make)
(cd m4/lib && make)
(cd lib && make install)


