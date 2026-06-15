#include "NoteBuilder.h"
#include "Note.h"

NoteBuilder::NoteBuilder() { note = new Note(); }

void NoteBuilder::NewNote() { note = new Note(); }

void NoteBuilder::LoadNote(Note &note) { this->note = &note; }

void NoteBuilder::SetFret(unsigned int fretNum) { note->fretNum = fretNum; }

void NoteBuilder::SetString(unsigned int stringNum) { note->stringNum = stringNum; }

void NoteBuilder::SetRhythm(Rhythm rhythm) { note->rhythm = rhythm; }

void NoteBuilder::SetLengthInTicks(unsigned int length) { note->lengthInTicks = length; }

bool NoteBuilder::ToggleHOPO(Note *prevNote) {
    if (note->CheckFlag(NOTE_HOPO_DEST)) {
        note->RemoveFlag(NOTE_HOPO_DEST);
        prevNote->RemoveFlag(NOTE_HOPO_SRC);
        return false;
    } else {
        note->AddFlag(NOTE_HOPO_DEST);
        prevNote->AddFlag(NOTE_HOPO_SRC);
        return true;
    }
}

bool NoteBuilder::ToggleSlide() {
    note->ToggleFlag(NOTE_SLIDE);
    return note->CheckFlag(NOTE_SLIDE);
}

bool NoteBuilder::ToggleUnpitchedSlide(unsigned int endFret = 0) {
    if (endFret > 0)
        note->slideFret = endFret;
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
