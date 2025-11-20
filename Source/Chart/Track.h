#pragma once

#include "Note.h"
#include <cstdint>
#include <map>

enum ARRANGEMENT_TYPE { ARR_LEAD, ARR_RHYTHM, ARR_BASS };

enum ARRANGEMENT_MOD { ARR_MODNONE, ARR_MODBONUS, ARR_MODALT };

class Track {
  public:
    void setArrangementType(ARRANGEMENT_TYPE);
    void setArragementModifier(ARRANGEMENT_MOD);
    ARRANGEMENT_TYPE getArrangementType();
    ARRANGEMENT_MOD getArrangementMod();

  private:
    // Map note id with its position in milliseconds along the song
    std::map<uint16_t, long> notes;
    ARRANGEMENT_TYPE arrType;
    ARRANGEMENT_MOD arrMod = ARR_MODNONE;
};
