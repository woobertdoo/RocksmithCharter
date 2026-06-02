/*
  ==============================================================================

    TimelinePanel.h
    Author:  jamieharris

  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/*
 */
class TimelinePanel : public juce::Component {
  public:
    TimelinePanel();
    ~TimelinePanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;

  private:
    juce::String currentSizeAsString;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelinePanel)
};
