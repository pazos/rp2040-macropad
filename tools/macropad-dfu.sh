#!/bin/bash

echo "dfu" > /dev/ttyACM0
sleep 5
cp -v $1 "/run/media/$(whoami)/RPI-RP2"
