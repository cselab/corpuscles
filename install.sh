#!/bin/sh

(cd tool && make) &&
(cd pkgconfig && make) &&
(cd m4/lib && make) &&
(cd lib && make install) &&
for i in bin/*/
do (cd $i; make install)
done
