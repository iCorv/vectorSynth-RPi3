/**
 * \class Key
 *
 *
 * \brief Key generates the tone of one key with 4 oscillators.
 *
 * This class generates the tone of one key with 4 oscillators, LPF, volume- and cut-off envelope.
 *
 * \author $Author: Corvin Jaedicke & Alberto Monciero $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2017/29/04 20:51:37 $
 *
 * Contact: c.jaedicke@math.tu-berlin.de
 *
 */

#pragma once

#define _USE_MATH_DEFINES

#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include "waveGen.h"
#include "moogLadderFilter.h"
#include "envelope.h"

using std::cout;
using std::endl;

class Key
{
public:
	friend class Midi2KeyHandler;
	/// Make a key with everything on default
	Key()
	: _velocity(0.0),
	_frequency(0.0),
	_alpha(0.5),
	_beta(0.5),
	_gamma(0.5),
	_keyNumber(-1),
	_cutOff(10000.0),
	isActive(false),
	volumeEnvelopeValue(0.0),
	filterEnvelopeValue(0.0),
	sineValue(0.0),
	sawValue(0.0),
	triValue(0.0),
	squValue(0.0) {
		sine.setType(CUSTOM_WAVE);
        square.setType(SQUARE);
        triangle.setType(TRIANGLE);
        sawtooth.setType(SAWTOOTH);
        filterLFO.setType(SINUS);
        filterLFO.setFrequency(2.0);
        filterLFO.setAmplitude(0.5);
	};

	/**
   	 * \brief Set new frequency for this key.
   	 * \param f New frequency
   	 */
	void setFrequency(float f);
	/**
   	 * \brief Get next sample.
   	 * \return sample
   	 * 
   	 * Slow method (deprecated)
   	 */
	float getNextSample();
	/**
   	 * \brief Get audio-buffer.
   	 * \param pBuffer Pointer to audio-buffer
   	 * \param nFrames Size of buffer
   	 * 
   	 * Fills the hole buffer, fast but makes filter LFO hard to implement, therefore deprecated.
   	 */
	void getNextSample(float * pBuffer, int nFrames);
	/**
   	 * \brief Get next sample.
   	 * \param pSample Pointer to sample
   	 * \param fLFO Filter LFO value
   	 * 
   	 * Calculate new sample and applie Filter LFO.
   	 */
	void getNextSample(float * pSample, float * fLFO);
	/// De-activates this key.
	void setFree();
	/// Activates this key.
	void setActive();
	/// Set the oscillator mix.
	void setOscillatorMix(float alpha, float beta, float gamma);
	/// Set key number (midi)
	void setKeyNumber(int keyNumber);
	/// Set Velocity
	void setVelocity(float velocity);
	/// Set Cut-off frequency
	void setCutOff(float value);
	/// Set volume envelope attack
	void setAttack(float value){volumeEnvelope.setAttack(value);}
	/// Set volume envelope decay
	void setDecay(float value){volumeEnvelope.setDecay(value);}
	/// Set volume envelope sustain
	void setSustain(float value){volumeEnvelope.setSustain(value);}
	/// Set volume envelope release
	void setRelease(float value){volumeEnvelope.setRelease(value);}
	/// Set filter resonance
	void setResonance(float value){moog.setResonance(value);}
	/// Set filter to low-pass 4
	void setLPF4(){moog.setFilter(moog.LPF4);}
	/// Set filter to low-pass 2
	void setLPF2(){moog.setFilter(moog.LPF2);}
	/// Set filter to high-pass 4
	void setHPF4(){moog.setFilter(moog.HPF4);}
	/// Set filter to high-pass 2
	void setHPF2(){moog.setFilter(moog.HPF2);}
	/// Set filter to band-pass 4
	void setBPF4(){moog.setFilter(moog.BPF4);}
	/// Set filter to band-pass 2
	void setBPF2(){moog.setFilter(moog.BPF2);}
	/// Reset key to no key number and WaveGen to phase zero etc.
	void reset();
	bool isActive; ///< true if Key is playing 
private:
    WaveGen sine; ///< Custom-Wave (should be re-named)
    WaveGen square; ///< Square-Wave
    WaveGen triangle; ///< Triangle-Wave
    WaveGen sawtooth; ///< Sawtooth-Wave
    MoogLadderFilter moog; ///< Moog-Ladder-Filter
    Envelope volumeEnvelope; ///< Volume envelope
    Envelope filterEnvelope; ///< Filter envelope
    WaveGen filterLFO; ///< Filter LFO (deprecated)
    float _velocity; ///< Velocity
    float _frequency; ///< Frequency
    int _keyNumber; ///< Key number (midi)
    float _alpha; ///< Determines the wave-mix of square- and custom-wave.
    float _beta; ///< Determines the wave-mix of saw- and triangle-wave. Always equal to alpha, only used for better understanding.
    float _gamma; ///< Mixes the two wave mixes to one signal.
    float _cutOff; ///< Cut-off frequency
    float volumeEnvelopeValue; ///< Volume envelope sample
	float filterEnvelopeValue; ///< Filter envelope sample
	float sineValue; ///< Custom-Wave sample
	float sawValue; ///< Saw-Wave sample
	float triValue; ///< Triangle sample
	float squValue; ///< Square-Wave sample
};