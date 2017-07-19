#include "key.h"


void Key::setFrequency(float f) {
		_frequency = f;
		sine.setFrequency(_frequency);
        square.setFrequency(_frequency);
        triangle.setFrequency(_frequency);
        sawtooth.setFrequency(_frequency);
}
float Key::getNextSample() {
	if (!isActive || volumeEnvelope.finishedEnvelopeCycle ) {
		filterEnvelope.enterStage(Envelope::ENVELOPE_STAGE_OFF);
		isActive = false;
		return 0.0;
	}
	volumeEnvelope.getNextSample(&volumeEnvelopeValue);
	filterEnvelope.getNextSample(&filterEnvelopeValue);
	//cout << filterEnvelopeValue << "\t" << endl;
	if(filterEnvelopeValue > 0)
		moog.setCutOff(filterEnvelopeValue * _cutOff);
	return moog.doFilter((_gamma * (sine.getNextAntiAliasedSample() *  _alpha + square.getNextAntiAliasedSample() *  (1-_alpha)) + 
    	(1-_gamma)*(triangle.getNextAntiAliasedSample() * _beta + sawtooth.getNextAntiAliasedSample() * (1-_beta))) * 
    	volumeEnvelopeValue * 
    	_velocity/127.0);
}

void Key::getNextSample(float * pSample, float * fLFO) {
	if (!isActive || volumeEnvelope.finishedEnvelopeCycle ) 
	{
		filterEnvelope.enterStage(Envelope::ENVELOPE_STAGE_OFF);
		isActive = false;
	} 
	else
	{
		volumeEnvelope.getNextSample(&volumeEnvelopeValue);
		filterEnvelope.getNextSample(&filterEnvelopeValue);
		//cout << filterEnvelopeValue << "\t" << endl;
		if(filterEnvelopeValue > 0)
			moog.setCutOff(filterEnvelopeValue * _cutOff * (*fLFO + 1.0));

		sineValue = squValue = sawValue = triValue = 0.0;

		sine.getNextAntiAliasedSample(&sineValue);
		square.getNextAntiAliasedSample(&squValue);
		triangle.getNextAntiAliasedSample(&triValue);
		sawtooth.getNextAntiAliasedSample(&sawValue);

		float sample = (_gamma * (sineValue *  _alpha + squValue *  (1-_alpha)) + 
	    	(1-_gamma)*(triValue * _beta + sawValue * (1-_beta))) * 
	    	volumeEnvelopeValue * 
	    	_velocity/127.0;
	    moog.doFilter(&sample);
	    *pSample += sample;
	}
}

void Key::getNextSample(float * pBuffer, int nFrames) {
	float sample = 0.0;
	for(int i = 0; i < nFrames; i++) {
		if (!isActive || volumeEnvelope.finishedEnvelopeCycle ) 
		{
			filterEnvelope.enterStage(Envelope::ENVELOPE_STAGE_OFF);
			isActive = false;
		} 
		else
		{
			volumeEnvelope.getNextSample(&volumeEnvelopeValue);
			filterEnvelope.getNextSample(&filterEnvelopeValue);
			//cout << filterEnvelopeValue << "\t" << endl;
			if(filterEnvelopeValue > 0.0 )
				moog.setCutOff(filterEnvelopeValue * _cutOff * (filterLFO.getNextSample()+1.0));

			sineValue = squValue = sawValue = triValue = 0.0;

			sine.getNextAntiAliasedSample(&sineValue);
			square.getNextAntiAliasedSample(&squValue);
			triangle.getNextAntiAliasedSample(&triValue);
			sawtooth.getNextAntiAliasedSample(&sawValue);

			sample = (_gamma * (sineValue *  _alpha + squValue *  (1-_alpha)) + 
		    	(1-_gamma)*(triValue * _beta + sawValue * (1-_beta))) * 
		    	volumeEnvelopeValue * 
		    	_velocity/127.0;

		    moog.doFilter(&sample);

		    pBuffer[i] += sample;
		}
	}
}

void Key::setOscillatorMix(float alpha, float beta, float gamma) {
	_alpha = alpha;
	_beta = beta;
	_gamma = gamma;
}
void Key::reset() {
	_keyNumber = -1;
	_velocity = 0;
	sine.reset();
	square.reset();
	triangle.reset();
	sawtooth.reset();
}
void Key::setFree() {
	isActive = false;
}
void Key::setActive() {
	isActive = true;
}
void Key::setKeyNumber(int keyNumber) {
	_keyNumber = keyNumber;
}
void Key::setVelocity(float velocity) {
	_velocity = velocity;
}
// value is in Hz
void Key::setCutOff(float value) {
	_cutOff = value;
	moog.setCutOff(_cutOff);
}