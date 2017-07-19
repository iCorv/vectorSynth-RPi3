# VectorSynth for Raspberry Pi 3
This is a student project for [Klangsynthese und -verarbeitung: Von der Theorie zur Programmierung](/http://www.ak.tu-berlin.de/menue/lehre/wintersemester_201617/klangsynthese_und_verarbeitung_von_der_theorie_zur_programmierung/).

# Libraries
The libraries needed for this to work properly are part of the Raspberry Pi 3 (RPi3) Image:

* [liblo](http://liblo.sourceforge.net/)
* [jackcpp](http://x37v.info/jack_cpp/)
* [rtmidi](http://www.music.mcgill.ca/~gary/rtmidi/)
* [libsndfile](http://www.mega-nerd.com/libsndfile)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp) 
* [boost](https://archlinuxarm.org/packages/armv7h/boost) 

# Images
Images for the RPi3 can be found [here](https://www2.ak.tu-berlin.de/~drunge/klangsynthese).
Please follow the [RPi3 installation instructions](https://archlinuxarm.org/platforms/armv8/broadcom/raspberry-pi-3) as a reference on how to install the latest image on a microSD card.

# Hardware
We use specific hardware, that has been tested for low-latency audio with JACK:

* RaspberryPi3
* [Renkforce USB sound card](https://www.conrad.de/de/soundkarte-extern-renkforce-externe-kopfhoereranschluesse-1406215.html)

# Sources
All sources can be found in the src/ directory.


# Links
* Raspberry Pi 3: https://www.raspberrypi.org/products/raspberry-pi-3-model-b/
* liblo (website): http://liblo.sourceforge.net/
* ArchLinux: https://aur.archlinux.org/
* liblo-git: https://aur.archlinux.org/packages/liblo-git/
* jackcpp (website): http://x37v.info/jack_cpp/
* jackcpp-git: https://aur.archlinux.org/packages/jackcpp-git/
* rtmidi (website): http://www.music.mcgill.ca/~gary/rtmidi/
* rtmidi: https://aur.archlinux.org/packages/rtmidi/
* libsndfile (website): http://www.mega-nerd.com/libsndfile
* libsndfile: https://archlinuxarm.org/packages/armv7h/libsndfile
* yaml-cpp (website): https://github.com/jbeder/yaml-cpp
* yaml-cpp: https://archlinuxarm.org/packages/armv7h/yaml-cpp
* boost (website): http://www.boost.org
* boost: https://archlinuxarm.org/packages/armv7h/boost
* RPi3 installation instructions: https://archlinuxarm.org/platforms/armv8/broadcom/raspberry-pi-3
* Renkforce USB sound card: https://www.conrad.de/de/soundkarte-extern-renkforce-externe-kopfhoereranschluesse-1406215.html
* JACK: http://jackaudio.org/
* systemd user service: https://git.sleepmap.de/software/uenv.git/about/