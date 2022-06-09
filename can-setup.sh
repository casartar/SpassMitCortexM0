#!/bin/sh

sudo modprobe can
sudo modprobe can-raw
sudo modprobe slcan
killall slcand
stty -F /dev/ttyAMA0 1152000
sudo slcan_attach -s6 -o /dev/ttyAMA0
sudo slcand ttyAMA0 slcan0
sudo ip link set slcan0 up
