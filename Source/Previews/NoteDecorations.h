#pragma once

#include <cstdint>
#include <juce_graphics/juce_graphics.h>

enum NoteDecorations {
    NOTE_DEC_UNPITCH_SLUP,
    NOTE_DEC_UNPITCH_SLDN,
    NOTE_DEC_PITCH_SLUP,
    NOTE_DEC_PITCH_SLDN,
    NOTE_DEC_HO,
    NOTE_DEC_PO,
    NOTE_DEC_TAP,
    NOTE_DEC_SLAP,
    NOTE_DEC_POP,
    NOTE_DEC_NAT_HARM,
    NOTE_DEC_ART_HARM,
};

class NotePreview {
  private:
    juce::Colour noteColor;
};
