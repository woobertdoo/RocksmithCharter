#pragma once

/* TECHNIQUE FLAGS */
#include <cstdint>

enum NoteFlag : uint32_t {
    NOTE_HOPO_SRC = 0,
    NOTE_HOPO_DEST = 1,
    NOTE_TAP = 2,
    NOTE_SLIDE = 3,
    NOTE_UNPITCHED_SLIDE = 4,
    NOTE_MUTE = 5,
    NOTE_PALMMUTE = 6,
    NOTE_LINKNEXT = 7,
    NOTE_VIBRATO = 8,
};

/* RHYTHM FLAGS */
enum RhythmFlag : int {
    RHYTHM_INVALID = -1,
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

    static const bool TupleIsEmpty(const Tuple &other) { //
        return other.den == 1 && other.num == 1;
    }
};

struct Rhythm {
    RhythmFlag baseVal;
    int dots = 0;
    Tuple tuple {1, 1};
};

class Note {
  public:
    Note();
    ~Note();

    const unsigned int GetFret() { return this->fretNum; };

    const unsigned int GetString() { return this->stringNum; };

    const unsigned int GetLengthInTicks() { return this->lengthInTicks; };

    const Rhythm GetRhythm() { return this->rhythm; };

    const unsigned int GetBeatOffsetTicks() { return this->beatOffsetTicks; };

    void SetFret(unsigned int fret) { this->fretNum = fret; };

    void SetString(unsigned int string) { this->stringNum = string; };

    void SetRhythm(Rhythm newRhythm) { this->rhythm = newRhythm; };

    void SetLengthInTicks(unsigned int length) { this->lengthInTicks = length; };

    void SetBeatOffsetTicks(unsigned int offset) { this->beatOffsetTicks = offset; };

    void AddFlag(NoteFlag flag) { this->flags &= (1 << flag); };

    void RemoveFlag(NoteFlag flag) { this->flags &= ~((uint32_t)1 << flag); };

    void ClearFlags() { this->flags = 0; };

    bool CheckFlag(NoteFlag flag) { return (this->flags & (1 << flag)) != 0; };

    void ToggleFlag(NoteFlag flag) { this->flags |= flag; };

  private:
    unsigned int fretNum;
    unsigned int stringNum;
    unsigned int slideFret;
    Rhythm rhythm;
    uint32_t flags;
    unsigned int lengthInTicks;
    unsigned int beatOffsetTicks;

    friend class NoteBuilder;
};
