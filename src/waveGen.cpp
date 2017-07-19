#include "waveGen.h"

WaveGen::WaveGen(float f, float a, float p,  int fS, TYPE type)
{
    setFrequency(f);
    amp          = a;
    phi          = p;
    fs           = fS;
    _type        = type;
    phiIncrement = TWO_PI * freq * (1.0 / (float) fs);
}

float WaveGen::getNextSample() {
    float thisVal = 0.0;
    if(!isFreqZero) {
        aliasedWaveGen(_type, &thisVal);
        // rotate to next step
        phi += phiIncrement;
        // wrap to 2 pi
        if(phi >= TWO_PI)
        {
            phi -= TWO_PI;
        }
        
    }
    return thisVal;
}

float WaveGen::getNextAntiAliasedSample() {

    float thisVal = 0.0;
    if(!isFreqZero) {
        float t = phi / TWO_PI;

        switch (_type)
        {
            case SINUS:
                aliasedWaveGen(SINUS, &thisVal);
                break;
            case CUSTOM_WAVE:
                aliasedWaveGen(CUSTOM_WAVE, &thisVal);
                break;
            case SAWTOOTH:
                aliasedWaveGen(SAWTOOTH , &thisVal);
                thisVal -= polyBlep(t);
                break;
            case SQUARE:
                aliasedWaveGen(SQUARE, &thisVal);
                thisVal += polyBlep(t);
                thisVal -= polyBlep(fmod(t + 0.5, 1.0));
                break;
            case TRIANGLE:
                aliasedWaveGen(TRIANGLE, &thisVal);
                break;
        }
        // rotate to next step
        phi += phiIncrement;
        // wrap to 2 pi
        while(phi >= TWO_PI)
        {
            phi -= TWO_PI;
        }
    }
    return thisVal;
}

void WaveGen::getNextAntiAliasedSample(float * pSample) {
    if(!isFreqZero) {
        float t = phi / TWO_PI;

        switch (_type)
        {
            case SINUS:
                aliasedWaveGen(SINUS, pSample);
                break;
            case CUSTOM_WAVE:
                aliasedWaveGen(CUSTOM_WAVE, pSample);
                break;
            case SAWTOOTH:
                aliasedWaveGen(SAWTOOTH ,pSample);
                *pSample -= polyBlep(t);
                break;
            case SQUARE:
                aliasedWaveGen(SQUARE, pSample);
                *pSample += polyBlep(t);
                *pSample -= polyBlep(fmod(t + 0.5, 1.0));
                break;
            case TRIANGLE:
                aliasedWaveGen(TRIANGLE, pSample);
                break;
        }
        // rotate to next step
        phi += phiIncrement;
        // wrap to 2 pi
        while(phi >= TWO_PI)
        {
            phi -= TWO_PI;
        }
    }
}

float WaveGen::polyBlep(float t)
{
    float dt = phiIncrement / TWO_PI;
    // 0 <= t < 1
    // begining of a period
    if (t < dt) {
        t /= dt;
        return t+t - t*t - 1.0;
    }
    // -1 < t < 0
    // ending of a period
    else if (t > 1.0 - dt) {
        t = (t - 1.0) / dt;
        return t*t + t+t + 1.0;
    }
    // returns 0 most of the time
    else return 0.0;
}

void WaveGen::aliasedWaveGenLUT(TYPE type, float * pSample) {
    switch (type)
    {
    case SINUS:
        *pSample += lookUpSine(&phi) * amp;
        break; 
    case SQUARE:
        if (phi <= M_PI)
            *pSample += 1;
        else
            *pSample -= 1;
        *pSample *= amp;
        break;
    case TRIANGLE:
        *pSample += lookUpTriangle(&phi) * amp;
        break;
    case SAWTOOTH:
        *pSample += lookUpSawtooth(&phi) * amp;
        break;
    case CUSTOM_WAVE:
        *pSample += lookUpCustomWave(&phi) * amp;
        break;
    }
}

void WaveGen::aliasedWaveGen(TYPE type, float * pSample) {
    switch (type)
    {
    case SINUS:
        *pSample += (sin(phi) * amp);
        break;  
    case SQUARE:
        if (phi <= M_PI)
            *pSample += 1;
        else
            *pSample -= 1;
        *pSample *= amp;
        break;
    case TRIANGLE:
        *pSample += 2.0 * (fabs(-1.0 + (2.0 * phi / TWO_PI)) - 0.5);
        *pSample *= amp;
        break;
    case SAWTOOTH:
        *pSample += (2.0 * phi / TWO_PI) - 1.0;
        *pSample *= amp;
        break;
    case CUSTOM_WAVE:
        *pSample += lookUpCustomWave(&phi) * amp;
        break;
    }
    
}

float WaveGen::lookUpCustomWave(float * pPhi) {
    int i = floor(*pPhi * factorLUT);
    float y1 = customWave[i];
    float y2 = customWave[i+1];
    return y1 + (y2 - y1) * (*pPhi * factorLUT - i);
}

float WaveGen::lookUpSawtooth(float * pPhi) {
    int i = floor(*pPhi * factorLUT);
    float y1 = saw[i];
    float y2 = saw[i+1];
    return y1 + (y2 - y1) * (*pPhi * factorLUT - i);
}

float WaveGen::lookUpTriangle(float * pPhi) {
    int i = floor(*pPhi * factorLUT);
    float y1 = tri[i];
    float y2 = tri[i+1];
    return y1 + (y2 - y1) * (*pPhi * factorLUT - i);
}

float WaveGen::lookUpSine(float * pPhi) {
    int i = floor(*pPhi * factorLUT);
    float y1 = sine[i];
    float y2 = sine[i+1];
    return y1 + (y2 - y1) * (*pPhi * factorLUT - i);
}

void WaveGen::updatePhi() {
    phiIncrement = TWO_PI * (freq / (float) fs);
}

float WaveGen::getFrequency() {
    return freq;
}

float WaveGen::getAmplitude() {
    return amp;
}

float WaveGen::getPhase() {
    return phi;
}

void WaveGen::setFrequency(float f) {
    freq = f;
    if(freq == 0.0) {
        isFreqZero = true;
    } else {
        isFreqZero = false;
    }
    updatePhi();
}

void WaveGen::setAmplitude(float a) {
    amp = a;
}

void WaveGen::setPhase(float p) {
    phi = p;
}
void WaveGen::setType(TYPE type)  {
    _type = type;
}
void WaveGen::reset() {
    freq = 0.0;
    phi = 0.0;
}