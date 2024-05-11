#!/bin/sh
# write flash to OTA buffer starting from 0x77000
python ../../TLSRPGM/TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x77000 $1





