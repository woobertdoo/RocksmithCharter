#pragma once

#include "Bar.h" 
#include "Beat.h"

class TempoEvent {
public:
    TempoEvent(Beat*, TimeSig newSig = {0, 0}, int newBPM = 0);
    ~TempoEvent();
};
