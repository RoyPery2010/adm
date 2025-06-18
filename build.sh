#!/bin/sh

set -xe


CC=${C:=/usr/bin/cc}
CFLAGS="-Wall -Wextra -Wswitch-enum -Wmissing-prototypes -pedantic -std=c11"
LIBS= 

$CC $CFLAGS -o pasm ./src/pasm.c $LIBS
$CC $CFLAGS -o depasm ./src/depasm.c $LIBS
$CC $CFLAGS -o adme ./src/adme.c $LIBS

for example in `find examples/ -name \*.pasm | sed "s/\.pasm//"`; do
    cpp -P "$example.pasm" > "$example.pasm.pp"
    ./pasm "$example.pasm.pp" "$example.adm"
    rm -r "$example.pasm.pp"
done