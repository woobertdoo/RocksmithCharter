#pragma once

#include "Note.h"

class NoteBuilder {
  public:
    NoteBuilder();
    void NewNote();
    void LoadNote(Note&);
    ~NoteBuilder();

    void SetFret(int);
    void SetString(int);
    void SetRhythm(Rhythm);
    void SetLengthInBeats(float);
    bool ToggleHOPO(Note&);
    bool ToggleTap();
    bool ToggleSlide(int);
    bool ToggleLinkNext();

    Note BuildNote();

  private:
    Note* note;
};
