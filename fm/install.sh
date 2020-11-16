#!/bin/sh

(cd tool
 make install)

(cd pkgconfig
 make install)

(cd lib
 make install)
