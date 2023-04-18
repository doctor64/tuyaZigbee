#!/bin/sh
echo "bootloader code"
srec_cat $1 -binary -crop 0x8 0xc -o - -hex-dump
echo "bootloader size"
srec_cat $1 -binary -crop 0x18 0x1c -o - -hex-dump
echo "firmware code"
srec_cat $1 -binary -crop 0x8008 0x800c -o - -hex-dump
echo "firmware size"
srec_cat $1 -binary -crop 0x8018 0x801c -o - -hex-dump
echo "factory reset. If not 0xFF erase NV on start"
srec_cat $1 -binary -crop 0xfc000 0xfc001 -o - -hex-dump
