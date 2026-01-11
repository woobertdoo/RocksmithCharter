/*
  ==============================================================================

    PreviewPanel.h
    Author:  jamieharris

  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/*
 */
class PreviewPanel : public juce::Component {
  public:
    PreviewPanel();
    ~PreviewPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;

  private:
    juce::String currentSizeAsString;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PreviewPanel)
};
