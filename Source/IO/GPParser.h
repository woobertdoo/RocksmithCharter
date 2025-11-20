#pragma once

#include <juce_core/juce_core.h>
#include <memory>
#include <vector>

using XmlVector = std::vector<std::unique_ptr<juce::XmlElement>>;

class GPParser {
  public:
    GPParser(std::unique_ptr<juce::XmlElement>& root) { rootNode = root.get(); }

    ~GPParser() {}

    juce::String GetGPVersion();

    void SetRoot(std::unique_ptr<juce::XmlElement>& root);

    XmlVector GetMasterBars();

    XmlVector GetBars(std::unique_ptr<juce::XmlElement>& masterBar);

    void FindTheWeirdElement();

  private:
    juce::XmlElement* rootNode;

    std::vector<int> splitIds(std::string ids);
};
