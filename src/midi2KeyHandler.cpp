#include "midi2KeyHandler.h"

void Midi2KeyHandler::mapMidi(MidiMan::midiMessage m)
{
    switch(m.byte1) {
        case 176:
            switch(m.byte2) {
                case 1: // positive y direction
                    if(!holdOn) {
                        //alpha = (float) (m.byte3-64/127.0; // pos range from 1 to 127
                        alpha = 0.5 + 0.5 * (float) m.byte3/127.0;
                        beta = alpha;
                        for(int i = 0; i < numberOfKeys; i++) {
                        	keys[i].setOscillatorMix(alpha, beta, gamma);
                        }
                    }
                    break;
                case 74: // negative y direction
                    if(!holdOn) {
                        alpha = 1.0 - (float) m.byte3/127.0;
                        //alpha = (float) m.byte3/127.0;
                        beta = alpha;
                        for(int i = 0; i < numberOfKeys; i++) {
                        	keys[i].setOscillatorMix(alpha, beta, gamma);
                        }
                    }
                    break;
                case 71: // Release
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setRelease((float) m.byte3 * 0.05);
                    }
                    break;
                case 73: // attack
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setAttack((float) m.byte3 / 127.0);
                    }
                    break;
                case 75: // decay
                    for(int i = 0; i < numberOfKeys; i++) {
                        //keys[i].setDecay((float) m.byte3 / 127.0);
                        keys[i].volumeEnvelope.setDecay((float) m.byte3 / 127.0);
                    }
                    break;
                case 72: // sustain
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setSustain((float) m.byte3 / 127.0);
                    }
                    break;
                case 76: //cutoff
                    for(int i = 0; i < numberOfKeys; i++) {
                    	cutOff = (exp((float) m.byte3 /127.0)-1.0) * maxCutOff + 20.0;
                        keys[i].setCutOff(cutOff);
                    }
                    break;
                case 77: // resonance
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setResonance((float) m.byte3 * 0.0315); // 4/127
                    }
                    break;
                case 78: // rate
                    maxCutOff = (float) m.byte3 * 78.7;
                    break;
                // case 5000: // depth
                //     depth = (float) m.byte3;
                //     break;
                default:
                    break;
            }
            break;
        case 224: // x direction
            if(!holdOn) {
                gamma = (float) m.byte3/127.0;
                for(int i = 0; i < numberOfKeys; i++) {
                        	keys[i].setOscillatorMix(alpha, beta, gamma);
                }
            }
            break;
        case 144:
            onKeyPressed(m.byte2, (float) m.byte3, freq[m.byte2-9]);
            //frequency = freq[m.byte2-9];
            break;
        case 128:
            onKeyReleased(m.byte2, (float) m.byte3);
            break;
        case 153:
            switch(m.byte2) {
                case 36:
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setLPF4();
                    }
                    break;
                case 40:
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setLPF2();
                    }
                    break;
                case 43:
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setHPF4();
                    }
                    break;
                case 42:
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setBPF4();
                    }
                    break;
                case 46:
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setBPF2();
                    }
                    break;
                case 45:
                    for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].setHPF2();
                    }
                    break;
                case 48:
                    holdOn = !holdOn;
                    break;
                default:
                    break;
            }
        case 177:
        	switch(m.byte2){
        		case 10:
        			globalLFO->setFrequency( (float) m.byte3 * 0.1575);
        			break;
        		case 91:
        			globalLFO->setAmplitude( (float) m.byte3 * 0.0787 );
        			break;
        		case 12:
        			cutOffLFO->setFrequency( (float) m.byte3 * 0.1575);
        			break;
        		case 93:
        			cutOffLFO->setAmplitude( (float) m.byte3 * 0.004);
        			break;
        		case 7:
        			for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].filterEnvelope.setAttack((float) m.byte3 / 127.0);
                    }
        			break;
        		case 1:
        			for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].filterEnvelope.setDecay((float) m.byte3 / 127.0 + 0.008); // +1/127, never zero!
                    }
        			break;
        		case 71:
        			for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].filterEnvelope.setSustain((float) m.byte3 / 127.0 + 0.008); // +1/127, never zero!
                    }
        			break;        	
        		case 74:
        			for(int i = 0; i < numberOfKeys; i++) {
                        keys[i].filterEnvelope.setRelease((float) m.byte3 * 0.0315 + 0.008);
                    }
        			break;        					
        	}
        break;
        case 145:
            switch(m.byte2){
                case 49:
                    globalLFO->setType(SINUS);
                    break;
                case 56:
                    globalLFO->setType(SAWTOOTH);
                    break;
                case 50:
                    globalLFO->setType(TRIANGLE);
                    break;
            }
            break;
        default:
            break;
    }

}

Key* Midi2KeyHandler::findFreeKey() {
    Key* freeKey = NULL;
    for(int i = 0; i < numberOfKeys; i++) {
        if(!keys[i].isActive) {
            freeKey = &(keys[i]);
            break;
        }
    }
    return freeKey;
}

void Midi2KeyHandler::onKeyPressed(int keyNumber, float velocity, float frequency) {
    Key* key = findFreeKey();
    if (!key) {
        return;
    }
    key->reset();
    key->setFrequency(frequency);
    key->setKeyNumber(keyNumber);
    key->setVelocity(velocity);
    key->setActive();
    key->volumeEnvelope.enterStage(Envelope::ENVELOPE_STAGE_ATTACK);
    key->filterEnvelope.enterStage(Envelope::ENVELOPE_STAGE_ATTACK);

}
void Midi2KeyHandler::onKeyReleased(int keyNumber, float velocity) {
    for(int i = 0; i < numberOfKeys; i++) {
        Key& key = keys[i];
        if(key.isActive && key._keyNumber == keyNumber ) {
            key.volumeEnvelope.enterStage(Envelope::ENVELOPE_STAGE_RELEASE);
            key.filterEnvelope.enterStage(Envelope::ENVELOPE_STAGE_RELEASE);
        }
    }
}

void Midi2KeyHandler::getNextSampleBuffer(float * buffer, int frames) {
    float cutOffLFOValue = 0.0;
    for(int j = 0; j < frames; j++) {
        buffer[j] = 0.0;
        cutOffLFOValue = cutOffLFO->getNextSample();
        for (int i = 0; i < numberOfKeys; i++) {
            Key& key = keys[i];
            key.getNextSample(&(buffer[j]), &cutOffLFOValue);
        }

        buffer[j] *= globalLFO->getNextSample()+1.0;
    }
}

