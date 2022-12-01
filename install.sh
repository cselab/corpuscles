#!/bin/sh

if ! gsl-config --version 2>/dev/null 1>/dev/null
then
    printf >&2 "install.sh: gsl-config is not found. GSL library should be installed\n."
    exit 1
fi

(cd tool && make) &&
(cd pkgconfig && make) &&
(cd m4/lib && make) &&
(cd lib && make install) &&
for i in bin/*/
do (cd $i; make install)
done
