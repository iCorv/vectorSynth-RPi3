/**
 * \class Midi2KeyHandler
 *
 *
 * \brief Manages incoming midi messages and handels all keys (voices).
 *
 * Messages from the midiMan-class are managed and changes in parameters are set in all keys. 
 * This class holds an array of keys and keeps track on pressed/released keys. 
 * Volume LFO and Cut-off LFO are applied on this level.
 *
 * \author $Author: Corvin Jaedicke & Alberto Monciero $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2017/29/04 19:49:12 $
 *
 * Contact: c.jaedicke@math.tu-berlin.de
 *
 */

#pragma once

#define _USE_MATH_DEFINES

#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include "midiman.h"
#include "key.h"

using std::cout;
using std::endl;

class Midi2KeyHandler
{
public:
	/// Midi-Key-Handler with default parameters applied
    Midi2KeyHandler() :
    alpha(0.5),
    beta(0.5),
    gamma(0.5), 
    holdOn(false),
    maxCutOff(10000.0),
    cutOff(10000.0) {
        globalLFO = new WaveGen( 0, 1, 0,  48000, SINUS);
        cutOffLFO = new WaveGen( 0, 0.5, 0,  48000, SINUS);
        for(int i = 0; i < numberOfKeys; i++) {
            keys[i].setCutOff(cutOff);
        }
    };

    /**
   	 * \brief Processes incoming midi messages.
   	 * \param m Midi message
   	 * 
   	 * This method maps incoming midi messages to their corresponding 
   	 * parameters and keys. This method is optimized for input from the cme Mobiltone U-Key 
   	 * (Keyboad Controller) and M-Audio Trigger Finger (Pad).
   	 */
    void mapMidi(MidiMan::midiMessage m);

    /**
   	 * \brief Activates a new key in the key-array
   	 * \param keyNumber The key number in the midi message
   	 * \param velocity The velocity applied to the key
   	 * \param frequency The frequency corresponding to this key number
   	 * 
   	 * If no key is available nothing happens, change numberOfKeys to get more keys.
   	 */
    void onKeyPressed(int keyNumber, float velocity, float frequency);
    /**
   	 * \brief De-activates the key in the key-array
   	 * \param keyNumber The key number in the midi message
   	 * \param velocity The velocity applied to the key
   	 * 
   	 * De-activates the key with corresponding keyNumber.
   	 */
    void onKeyReleased(int keyNumber, float velocity);
    /**
   	 * \brief Fills the audio-out buffer with new samples.
   	 * \param buffer Pointer to the audio-buffer
   	 * \param frames Size of the buffer
   	 * 
   	 * Iterates through all keys and adds up the new sample value.
   	 * Volume LFO is applied in a last step.
   	 * Cut-off LFO is handed to each key.
   	 */
    void getNextSampleBuffer(float* buffer, int frames);

    // wave mix
    float alpha; ///< Determines the wave-mix of square- and custom-wave.
    float beta; ///< Determines the wave-mix of saw- and triangle-wave. Always equal to alpha, only used for better understanding.
    float gamma; ///< Mixes the two wave mixes to one signal.

private:
    WaveGen *globalLFO; ///< Volume LFO
    WaveGen *cutOffLFO; ///< Cut-off LFO
    bool holdOn; ///< Flag, if true alpha, beta and gamme are fixed.
    float maxCutOff; ///< max Cut-off frequency (10kHz)
    float cutOff; //y< Current Cut-off frequency
    const float freq[88] = {27.5000000000000,
                                29.1353000000000,
                                30.8677000000000,
                                32.7032000000000,
                                34.6479000000000,
                                36.7081000000000,
                                38.8909000000000,
                                41.2035000000000,
                                43.6536000000000,
                                46.2493000000000,
                                48.9995000000000,
                                51.9130000000000,
                                55.0,
                                58.2705000000000,
                                61.7354000000000,
                                65.4064000000000,
                                69.2957000000000,
                                73.4162000000000,
                                77.7817000000000,
                                82.4069000000000,
                                87.3071000000000,
                                92.4986000000000,
                                97.9989000000000,
                                103.826000000000,
                                110.0,
                                116.541000000000,
                                123.471000000000,
                                130.813000000000,
                                138.591000000000,
                                146.832000000000,
                                155.563000000000,
                                164.814000000000,
                                174.614000000000,
                                184.997000000000,
                                195.998000000000,
                                207.652000000000,
                                220.0,
                                233.082000000000,
                                246.942000000000,
                                261.626000000000,
                                277.183000000000,
                                293.665000000000,
                                311.127000000000,
                                329.628000000000,
                                349.228000000000,
                                369.994000000000,
                                391.995000000000,
                                415.305000000000,
                                440.0,
                                466.164000000000,
                                493.883000000000,
                                523.251000000000,
                                554.365000000000,
                                587.330000000000,
                                622.254000000000,
                                659.255000000000,
                                698.456000000000,
                                739.989000000000,
                                783.991000000000,
                                830.609000000000,
                                880.0,
                                932.328000000000,
                                987.767000000000,
                                1046.50000000000,
                                1108.73000000000,
                                1174.66000000000,
                                1244.51000000000,
                                1318.51000000000,
                                1396.91000000000,
                                1479.98000000000,
                                1567.98000000000,
                                1661.22000000000,
                                1760.0,
                                1864.66000000000,
                                1975.53000000000,
                                2093.0,
                                2217.46000000000,
                                2349.32000000000,
                                2489.02000000000,
                                2637.02000000000,
                                2793.83000000000,
                                2959.96000000000,
                                3135.96000000000,
                                3322.44000000000,
                                3520.0,
                                3729.31000000000,
                                3951.07000000000,
                                4186.01000000000}; ///< Frequency of the different keys

    static const int numberOfKeys = 24; ///< max number of keys that can be active at one time. Including keys that are in release-mode.
    Key keys[numberOfKeys]; ///< Array holding all keys
    /**
   	 * \brief Find a free key in the key-array
   	 * \return Pointer to the free key.
   	 */
    Key* findFreeKey();

};
