#include "TempoEvent.h"

TempoEvent::TempoEvent(Beat* parentBeat, TimeSig newSig, int newBPM) {
    parentBeat->setAnchored(true);
    if(newSig.num != 0 && newSig.denom != 0) {
        
    }

    if(newBPM != 0) parentBeat->setBPM(newBPM);
}
