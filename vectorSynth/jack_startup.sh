#!/bin/sh

# kill (possibly) running servers
killall -9 jackd

## Set the CPU scaling governor to performance
echo -n performance | sudo tee /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

## Shutdown wifi if its not needed
sudo ifconfig wlan0 down

## Onboard Sound:
# jackd -dalsa -r44100 -p2048 -n3 &

## Focusrite:
 jackd -P80 -p16 -t2000 -dalsa -d hw:USB -p256 -n3 -r48000 -s &

## Renkforce:
# jackd -P80 -p16 -t2000 -dalsa -d hw:Device -p128 -n3 -r48000 -s &
