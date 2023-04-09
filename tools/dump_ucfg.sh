#!/bin/sh
srec_cat $1 -binary -crop 0xfc000 0xfc100 -o - -hex-dump
echo "pre-install code"
srec_cat $1 -binary -crop 0xfd000 0xfd010 -o - -hex-dump
echo "factory reset. If not 0xFF erase NV on start"
srec_cat $1 -binary -crop 0xfc000 0xfc001 -o - -hex-dump
