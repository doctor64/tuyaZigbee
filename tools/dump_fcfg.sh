#!/bin/sh
# dump F_cfg_info area from 1M firmware
srec_cat $1 -binary -crop 0xfe000 0xfe200 -o - -hex-dump
echo "frequency offset calibration"
srec_cat $1 -binary -crop 0xfe000 0xfe001 -o - -hex-dump
echo "BLE TP_GAIN_0, TP_GAIN_1"
srec_cat $1 -binary -crop 0xfe040 0xfe042 -o - -hex-dump
echo "Zigbee TP_GAIN_0, TP_GAIN_1"
srec_cat $1 -binary -crop 0xfe042 0xfe044 -o - -hex-dump
echo "32K_COUNTER_CALIBRATION - not supported?"
srec_cat $1 -binary -crop 0xfe080 0xfe090 -o - -hex-dump
echo "CFG_ADC_CALIBRATION - not supported?"
srec_cat $1 -binary -crop 0xfe0c0 0xfe0d0 -o - -hex-dump
echo "24M_CRYSTAL_CALIBRATION - not supported?"
srec_cat $1 -binary -crop 0xfe100 0xfe110 -o - -hex-dump
echo "T_SENSOR_CALIBRATION - not supported?"
srec_cat $1 -binary -crop 0xfe140 0xfe150 -o - -hex-dump
echo "UID-based Firmware Encryption data"
srec_cat $1 -binary -crop 0xfe180 0xfe190 -o - -hex-dump
echo "VDD_F calibration DCDC trim LDO trim"
srec_cat $1 -binary -crop 0xfe1c0 0xfe1c2 -o - -hex-dump



