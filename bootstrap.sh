#!/bin/sh

echo "This script basically just runs autoreconf, which you can do yourself."

set -ex

test -x `which autoreconf` && autoreconf -fvi --warnings=all
test -e autoconf.h.in~ && rm -f autoconf.h.in~
test -e autom4te.cache && rm -rf autom4te.cache
