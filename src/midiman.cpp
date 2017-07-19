#include "midiman.h"

bool MidiMan::done;

MidiMan::MidiMan()
{
    midiin = new RtMidiIn(RtMidiIn::Api::UNSPECIFIED ,std::string("RtMidi Input Client"),(unsigned int) 100);
    unsigned int nPorts = midiin->getPortCount();
    midiin->openPort( 0 );
    // Ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( true, true, true );
    done = false;
}



void MidiMan::setVerbose()
{
    isVerbose = true;

}

MidiMan::midiMessage MidiMan::get_rtmidi()
{
   	MidiMan::midiMessage mm;
   	std::vector<unsigned char>  a;
   	midiin->getMessage(&a);
    int nBytes = a.size();

    // only do something if 3 bytes are received
    if(nBytes == 3) {
    // only give feedback if 'verbose-mode' is active
        if( isVerbose == true  ) {
            for (int i=0; i<nBytes; i++ )
                std::cout << std::dec << (int)a[i] << " | " ;
                std::cout <<  std::endl;
        }
        mm.byte1 = a[0];
        mm.byte2 = a[1];
        mm.byte3 = a[2];
        mm.hasBeenProcessed = true;
    }
    return mm;
}




//////////////////////////////////////////////////////////////////
// FLUSH
//////////////////////////////////////////////////////////////////

void MidiMan::flushProcessedMessages()
{

    val.clear();


}


int MidiMan::getNumFaderMessages()
{

    int v  = (int) val.size();

    return v;
}




