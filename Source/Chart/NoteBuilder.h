#pragma once

#include "Note.h"

class NoteBuilder {
  public:
    NoteBuilder();
    void NewNote();
    void LoadNote(Note &);
    ~NoteBuilder();

    void SetFret(unsigned int);
    void SetString(unsigned int);
    void SetRhythm(Rhythm);
    void SetLengthInTicks(unsigned int);
    bool ToggleHOPO(Note *);
    bool ToggleTap();
    bool ToggleSlide();
    bool ToggleUnpitchedSlide(unsigned int);
    bool ToggleLinkNext();

    Note BuildNote();

  private:
    Note *note;
};
