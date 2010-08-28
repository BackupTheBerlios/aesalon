#!/bin/sh

cat monitor.conf | grep -v calloc-offset > monitor.conf.temp
mv monitor.conf.temp monitor.conf

echo -n 'set calloc-offset=' >> monitor.conf
nm -D `cat /proc/self/maps | grep libc-2 | head -n 1 | awk '{print $6}'` | grep ' calloc' | awk '{print $1}' | sed 's/^0*/0x/' >> monitor.conf

