#!/bin/sh

x86_64='x86_64'
x86='i686'

PLATFORM=`uname -m`


if [[ $PLATFORM == $x86_64 ]]; then
    echo 1;
elif [[ $PLATFORM == $x86 ]]; then
    echo 0;
fi

