/**
 * \class VectorSynth
 *
 *
 * \brief This class combines all parts to a Vector-Synthesizer like the Prophet-600!
 *
 * \author $Author: Corvin Jaedicke & Alberto Monciero $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2017/29/04 21:14:27 $
 *
 * Contact: c.jaedicke@math.tu-berlin.de
 *
 */



#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <jackaudioio.hpp>

#include "../src/midiman.h"
#include "../src/midi2KeyHandler.h"

using std::cout;
using std::endl;



class VectorSynth: public JackCpp::AudioIO {

private:

    MidiMan *midiMan;
    Midi2KeyHandler *keyHandler;

public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){

        while(processMIDI()) {}
        keyHandler->getNextSampleBuffer(outBufs[0], nframes);
        // return 0 on success
        return 0;
    }

    /// Constructor
    VectorSynth() :
        JackCpp::AudioIO("vectorSynth", 0,1){
        //reserveInPorts(2);
        reserveOutPorts(2);
        /// allocate a new midi manager
        midiMan = new MidiMan();
        keyHandler = new Midi2KeyHandler();
        // debug on
        //midiMan->setVerbose();
    }

    bool processMIDI() {
        /// process midi messages
        MidiMan::midiMessage val = midiMan->get_rtmidi();

        if(val.hasBeenProcessed)
        {
            keyHandler->mapMidi(val);
            // flush all messages
            midiMan->flushProcessedMessages();
        }
        return val.hasBeenProcessed;
    }

};

///
///
///
int main(int argc, char *argv[]){

    /// initial ports from constructor created here.
    VectorSynth * t = new VectorSynth();


    /// activate the client
    t->start();

    /// connect sine ports to stereo ports
    t->connectToPhysical(0,0);		// connects this client out port 0 to physical destination port 0
    t->connectToPhysical(0,1);		// connects this client out port 1 to physical destination port 1

    ///print names
    cout << "outport names:" << endl;
    for(unsigned int i = 0; i < t->outPorts(); i++)
        cout << "\t" << t->getOutputPortName(i) << endl;

    //cout << "Buffersize:" << t->isRealTime() << "\t" << endl;

    /// run for EVER
    sleep(-1);

    /// never reached:!=
    t->disconnectInPort(0);	// Disconnecting ports.
    t->disconnectOutPort(1);
    t->close();	// stop client.
    delete t;	// always clean up after yourself.
    exit(0);
}

