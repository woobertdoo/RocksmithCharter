#pragma once

#include "Beat.h"
#include <juce_core/juce_core.h>
#include <memory>
#include <vector>

struct TimeSig {
    int num = 0;
    int denom = 0;
};

class Bar {
  public:
    Bar(TimeSig);
    Bar(std::vector<Beat>, TimeSig);
    ~Bar();
    void AddBeat(Beat* beat);
    void SetTimeSig(TimeSig newTimeSig) { this->timeSig = newTimeSig; }
    const TimeSig GetTimeSig() const { return this->timeSig; }
    const int GetID() { return barId; }


  private:
    int numSubBeats;
    void setNumBeats();
    int numBeats;
    int barId;
    std::vector<Beat*> beats;
    TimeSig timeSig;
};
