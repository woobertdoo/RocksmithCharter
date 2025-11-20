#pragma once

#include <juce_core/juce_core.h>
#include <memory>
#include <vector>

class GPParser {
  public:
    static juce::String GetGPVersion(std::unique_ptr<juce::XmlElement>& root);

    static std::vector<std::unique_ptr<juce::XmlElement>>
    GetMasterBars(std::unique_ptr<juce::XmlElement>& root);

    static void FindTheWeirdElement(std::unique_ptr<juce::XmlElement>& root);
};
