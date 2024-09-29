#!/bin/bash

xset -dpms
xset s off
openbox-session &
start-pulseaudio-x11

while true; do
	cd /var/barcode-readers
	/var/barcode-readers/barcode-readerss
done
