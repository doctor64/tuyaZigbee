#!/bin/sh
srec_cat $1 -binary -crop 0xff000 0xff008 -o - -hex-dump
