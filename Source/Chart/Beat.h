#pragma once

#include "Note.h"
#include <memory>
#include <vector>

class Beat {
public:
    Beat();
    Beat(const Beat& other);
    ~Beat();
    const bool getAnchored();
    const bool getSubBeat();
    const int getBPM();
    void calculateTickLengthMS();
    void calculateBeatLengthMS(int);
    const int getTickLengthMS() { return tickLengthMS;}
    const int getBeatLengthMS() { return beatLengthMS; }
    void setAnchored(bool anchored) { isAnchored = anchored; } 
    void setSubBeat(bool subBeat) { isSubBeat = subBeat; }
    void setBPM(int newBPM) { bpm = newBPM;}
private:
    int tickLengthMS;
    int beatLengthMS;
    bool isAnchored;
    bool isSubBeat;
    float bpm;
    std::vector<Note*> notesStartingInBeat;
};
