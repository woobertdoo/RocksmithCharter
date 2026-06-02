#include "Bar.h"
#include <juce_core/juce_core.h>
#include <memory>

Bar::Bar(std::vector<Beat> inBeats, TimeSig timeSig) {
    
    for(Beat b : inBeats)
        AddBeat(&b);

    this->SetTimeSig(timeSig);
    this->setNumBeats();
}

Bar::Bar(TimeSig timeSig) { 
    this->SetTimeSig(timeSig); 
    this->setNumBeats();
}

void Bar::setNumBeats() {

    // Bars with 8th notes or shorter base unit dont get subbeats
    // All other base units get subbeats so the smallest subbeat is one eigth note long

    switch (this->timeSig.denom) {
        case 1:
            this->numSubBeats = 8;
            break;
        case 2:
            this->numSubBeats = 4;
            break;
        case 4:
            this->numSubBeats = 2;
            break;
        default:
            this->numSubBeats = 1;
            break;
    }

    this->numBeats = this->timeSig.num * this->numSubBeats;
}

void Bar::AddBeat(Beat* beat) {
    this->beats.push_back(beat); 
}
