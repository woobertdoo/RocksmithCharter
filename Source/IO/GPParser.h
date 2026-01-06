#pragma once

#include <juce_core/juce_core.h>
#include <memory>
#include <unordered_map>
#include <vector>

using XmlVector = std::vector<std::unique_ptr<juce::XmlElement>>;

class GPParser {
  public:
    GPParser(std::unique_ptr<juce::XmlElement>& root) { rootNode = root.get(); }

    ~GPParser() {}

    juce::String GetGPVersion();

    void SetRoot(std::unique_ptr<juce::XmlElement>& root);

    XmlVector GetMasterBars(std::string trackName);

    XmlVector GetBars(std::vector<int> barIds);

    void PopulateTrackMap(std::unique_ptr<juce::XmlElement>& root);

    void FindTheWeirdElement();

  private:
    juce::XmlElement* rootNode;

    int masterBarLength;

    std::unordered_map<std::string, int> barOffsets;

    std::vector<int> splitIds(std::string ids);

    std::vector<int> getTrackBars(std::string trackName);
};
