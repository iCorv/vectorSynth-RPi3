/**
 * \class Envelope
 *
 *
 * \brief Envelope, controlled via ADSR.
 *
 * Taken from http://www.martin-finke.de/blog/articles/audio-plugins-011-envelopes/ with permission for private use only.
 *
 * \author $Author: Martin Finke, modified by Corvin Jaedicke & Alberto Monciero $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2017/29/04 21:28:10 $
 *
 * Contact: c.jaedicke@math.tu-berlin.de
 *
 */

#pragma once

#include <cmath>

class Envelope {
public:
    enum EnvelopeStage { ///< Envelope stages
        ENVELOPE_STAGE_OFF = 0,
        ENVELOPE_STAGE_ATTACK,
        ENVELOPE_STAGE_DECAY,
        ENVELOPE_STAGE_SUSTAIN,
        ENVELOPE_STAGE_RELEASE,
        kNumEnvelopeStages
    };
    /// Move to new stage
    void enterStage(EnvelopeStage newStage);
    /**
   	 * \brief Get next sample.
   	 * \return sample
   	 * 
   	 * Slow method (deprecated)
   	 */
    double getNextSample();
    /**
   	 * \brief Get next sample.
   	 * \param pSample Pointer to sample
   	 * 
   	 * Calculate new sample.
   	 */
    void getNextSample(float * pSample);
    /// New sample rate...
    void setSampleRate(double newSampleRate);
    /// Get current stage in envelope
    inline EnvelopeStage getCurrentStage() const { return currentStage; };
    const double minimumLevel; ///< min volume 
    /// Default envelope
    Envelope() :
    minimumLevel(0.0001),
    currentStage(ENVELOPE_STAGE_OFF),
    finishedEnvelopeCycle(true),
    currentLevel(minimumLevel),
    multiplier(1.0),
    sampleRate(48000.0),
    currentSampleIndex(0),
    nextStageSampleIndex(0) {
        stageValue[ENVELOPE_STAGE_OFF] = 0.0;
        stageValue[ENVELOPE_STAGE_ATTACK] = 0.01;
        stageValue[ENVELOPE_STAGE_DECAY] = 0.5;
        stageValue[ENVELOPE_STAGE_SUSTAIN] = 0.1;
        stageValue[ENVELOPE_STAGE_RELEASE] = 1.0;
    };
 	bool finishedEnvelopeCycle;
 	/// Set Attack
    void setAttack(double val){stageValue[ENVELOPE_STAGE_ATTACK] = val;}
    /// Set Decay
    void setDecay(double val){stageValue[ENVELOPE_STAGE_DECAY] = val;}
    /// Set Sustain
    void setSustain(double val){stageValue[ENVELOPE_STAGE_SUSTAIN] = val;}
    /// Set Release
    void setRelease(double val){stageValue[ENVELOPE_STAGE_RELEASE] = val;}
private:
    EnvelopeStage currentStage; ///< Current stage
    double currentLevel; ///< Current level
    double multiplier; ///< Value to be applied on target sample
    double sampleRate; ///< sample rate
    double stageValue[kNumEnvelopeStages]; ///< stage number
    /**
   	 * \brief Calculates the value to be applied on target sample
   	 * \param startLevel starting point of level change
   	 * \param endLevel end point of level change
	 * \param lengthInSamples length of the change ramp
   	 * 
   	 */
    void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);
    unsigned long long currentSampleIndex; ///< current index of sample
    unsigned long long nextStageSampleIndex; ///< number of next envelope stage
};
