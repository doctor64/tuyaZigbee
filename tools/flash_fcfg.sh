#!/bin/sh
# flash F_cfg_info area and mac from file to addr 0xfe000, size 0x2000 (8192)
python ../../TLSRPGM/TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 we 0xfe000 $1





