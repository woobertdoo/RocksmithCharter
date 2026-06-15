#pragma once

#include "Note.h"
#include "NoteBuilder.h"
#include <juce_core/juce_core.h>
#include <memory>
#include <unordered_map>
#include <vector>

using XmlVector = std::vector<std::unique_ptr<juce::XmlElement>>;

enum SlideFlags : size_t {
    SHIFT_SLIDE = 1,
    LEGATO_SLIDE = 2,
    SLIDEOUT_DOWN = 4,
    SLIDEOUT_UP = 8,
    SLIDEIN_BELOW = 16,
    SLIDEIN_ABOVE = 32
};

class GPParser {
  public:
    GPParser(std::unique_ptr<juce::XmlElement> &root) { rootNode = root.get(); }

    ~GPParser() {}

    juce::String GetGPVersion();

    void SetRoot(std::unique_ptr<juce::XmlElement> &root);

    XmlVector GetMasterBars(std::string trackName);

    XmlVector GetBars(std::vector<int> barIds);
    XmlVector GetVoices(std::vector<int> voiceIds);

    std::vector<Note *> ParseNotes(int beatId);

    void PopulateTrackMap(std::unique_ptr<juce::XmlElement> &root);

    void FindTheWeirdElement();

  private:
    juce::XmlElement *rootNode;

    int masterBarLength;

    RhythmFlag NoteValueToFlag(juce::String noteValue);

    unsigned int getNumSixteenths(const Rhythm &);

    unsigned int getTickLength(const Rhythm &, int &);

    std::unordered_map<std::string, int> barOffsets;

    std::vector<int> splitIds(std::string ids);

    std::vector<int> getTrackBars(std::string trackName);

    NoteBuilder nb;
};
