/*
  ==============================================================================

    MainComponent.h
    Created: 23 Apr 2025 10:11:22pm
    Author:  jamieharris

  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/*
 */
class MainComponent : public juce::Component {
  public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

  private:
    juce::String currentSizeAsString;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
