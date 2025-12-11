#!/bin/sh

sed '/^\s*#/d' test/data/tz.conf |
while IFS= read -r l
do
	TZ=$l date
done
