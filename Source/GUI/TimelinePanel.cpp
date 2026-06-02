#include "TimelinePanel.h"

//==============================================================================
TimelinePanel::TimelinePanel() {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

TimelinePanel::~TimelinePanel() {}

void TimelinePanel::paint(juce::Graphics& g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.fillAll(juce::Colours::pink);
    g.setColour(juce::Colours::darkblue);
    g.setFont(20.0f);
    g.drawText("Timeline", getLocalBounds(), juce::Justification::centred,
               true);
}

void TimelinePanel::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..
}
