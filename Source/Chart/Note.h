#pragma once

/* TECHNIQUE FLAGS */
#include <cstdint>
enum NoteFlag {
    NOTE_HOPO_SRC,
    NOTE_HOPO_DEST,
    NOTE_TAP,
    NOTE_SLIDE,
    NOTE_UNPITCHED_SLIDE,
    NOTE_MUTE,
    NOTE_PALMMUTE,
    NOTE_LINKNEXT,
    NOTE_VIBRATO,
};

/* RHYTHM FLAGS */
enum RhythmFlag {
    RHYTHM_WHOLE = 1,
    RHYTHM_HALF = 2,
    RHYTHM_QUARTER = 4,
    RHYTHM_EIGHTH = 8,
    RHYTHM_SIXTEENTH = 16,
    RHYTHM_THIRTYSECOND = 32
};

/*
 * @param: num is the amount of notes
 * @param: den is the note space
 * EG: A standard triplet has num 3 and den 2,
 * 3 notes in the space of 2
 */
struct Tuple {
    int num;
    int den;
};

struct Rhythm {
    int baseVal;
    int dots = 0;
    Tuple tuple{1, 1};
};

class Note {
  public:
    Note();
    ~Note();

    const int GetFret() { return this->fretNum; };
    const int GetString() { return this->stringNum; };
    const float GetLengthInBeats() { return this->lengthInBeats; };
    const Rhythm GetRhythm() { return this->rhythm; };
    void SetFret(int);
    void SetString(int);
    void SetRhythm(Rhythm rhythm) { this->rhythm = rhythm; };
    void SetLengthInBeats(float length) { this->lengthInBeats = length; };

    void AddFlag(NoteFlag flag) { this->flags &= (1 << flag); };
    void RemoveFlag(NoteFlag flag) { this->flags &= ~(1 << flag); };
    void ClearFlags() { this->flags = 0; };
    bool CheckFlag(NoteFlag flag) { return (this->flags & (1 << flag)) != 0; };
    void ToggleFlag(NoteFlag flag) { this->flags |= flag; };

  private:
    int fretNum;
    int stringNum;
    int slideFret;
    Rhythm rhythm;
    uint32_t flags;
    float lengthInBeats;

    friend class NoteBuilder;
};
