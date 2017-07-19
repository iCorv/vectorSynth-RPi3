#!/bin/sh

g++ -O3 -std=c++11 vectorSynth.cpp ../src/filter.cpp ../src/moogLadderFilter.cpp ../src/midiman.cpp ../src/waveGen.cpp ../src/key.cpp ../src/envelope.cpp ../src/midi2KeyHandler.cpp -ljack -ljackcpp -lrtmidi  -o vectorSynth
