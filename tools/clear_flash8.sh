#!/bin/sh
# write flash starting from 0x8000
python ../../TLSRPGM/TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m es 0x8000 0x100



