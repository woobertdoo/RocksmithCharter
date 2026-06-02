#pragma once

#include "FileCodes.h"
#include <juce_core/juce_core.h>
#include <memory>

class GPExtractor {
  public:
    GPError ExtractScore(juce::String gpName,
                         std::unique_ptr<juce::XmlElement>& scoreRoot);
};
