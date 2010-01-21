#!/bin/sh
echo "# Automatically generated on " $(date) > version_info
echo "set(AESALON_MAJOR_VERSION 0)" >> version_info
echo "set(AESALON_MINOR_VERSION 0)" >> version_info
echo "set(AESALON_PATCHLEVEL 0)" >> version_info

git tag -l 'version_*'|sort -r | head -n 1 | awk -F '_' '{print $2}' | awk -F '.' '{ print "set(AESALON_MAJOR_VERSION " $1 ")\nset(AESALON_MINOR_VERSION " $2 ")\nset(AESALON_PATCHLEVEL " $3 ")" }' >> version_info

