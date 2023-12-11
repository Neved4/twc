#!/bin/sh

sed '/^\s*#/d' tools/samples.tz |
while IFS= read -r l
do
    TZ=$l date
done
