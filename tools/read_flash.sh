#!/bin/sh
# dump all flash from 0x0 to len 0x100000 (1M)
python ../../TLSRPGM/TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 rf 0 0x100000 $1





