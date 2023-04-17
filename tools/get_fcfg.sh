#!/bin/sh
# get F_cfg_info area and mac from 1M firmware (get addresses 0xfe000-0x100000)
srec_cat $1 -binary -crop 0xfe000 0x100000 -offset - 0xfe000 -o fcfg_mac.bin -binary




