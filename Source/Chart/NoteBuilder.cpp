#include "NoteBuilder.h"
#include "Note.h"

NoteBuilder::NoteBuilder() { note = new Note(); }

void NoteBuilder::NewNote() { note = new Note(); }
void NoteBuilder::LoadNote(Note& note) { this->note = &note; }

void NoteBuilder::SetFret(int fretNum) { note->fretNum = fretNum; }

void NoteBuilder::SetString(int stringNum) { note->stringNum = stringNum; }

void NoteBuilder::SetRhythm(Rhythm rhythm) { note->rhythm = rhythm; }

void NoteBuilder::SetLengthInBeats(float length) {
    note->lengthInBeats = length;
}

bool NoteBuilder::ToggleHOPO(Note& prevNote) {
    if (note->CheckFlag(NOTE_HOPO_DEST) == NOTE_HOPO_DEST) {
        note->RemoveFlag(NOTE_HOPO_DEST);
        prevNote.RemoveFlag(NOTE_HOPO_SRC);
        return false;
    } else {
        note->ToggleFlag(NOTE_HOPO_DEST);
        prevNote.ToggleFlag(NOTE_HOPO_SRC);
        return true;
    }
}

bool NoteBuilder::ToggleSlide(int slideFret = -1) {
    note->slideFret = slideFret;
    note->ToggleFlag(NOTE_SLIDE);
    return note->CheckFlag(NOTE_SLIDE);
}

bool NoteBuilder::ToggleTap() {
    note->ToggleFlag(NOTE_TAP);
    return note->CheckFlag(NOTE_TAP);
}

bool NoteBuilder::ToggleLinkNext() {
    note->ToggleFlag(NOTE_LINKNEXT);
    return note->CheckFlag(NOTE_LINKNEXT);
}

Note NoteBuilder::BuildNote() { return *note; }
