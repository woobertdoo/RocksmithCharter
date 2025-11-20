#include "Bar.h"

Bar::Bar(std::vector<Note*> notes, TimeSig timeSig) {
    this->notes = notes;
    this->SetTimeSig(timeSig);
}

Bar::Bar(TimeSig timeSig) { this->SetTimeSig(timeSig); }

void Bar::AddNote(Note& note) { this->notes.push_back(&note); }
