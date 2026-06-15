#include "Beat.h"
#include "constants.h"

Beat::Beat() {}

Beat::Beat(const Beat &other) {
    // std::move(other.notesStartingInBeat.begin(), other.notesStartingInBeat.end(), this->notesStartingInBeat);
}

void Beat::calculateBeatLengthMS(int numSubBeats) {

    /*
     * BPM = full beats per minute
     * BPM / 60 = full beats per second
     * 60 / BPM = seconds per Full Beat
     * SPB / numSubBeats = seconds per subbeat
     * SPSB * 1000 = milliseconds per subBeat
     */

    this->beatLengthMS = (60 / this->bpm) / numSubBeats * 1000;
    calculateTickLengthMS();
}

void Beat::calculateTickLengthMS() { this->tickLengthMS = this->beatLengthMS / CHART_TICKS_PER_QUARTER; }
