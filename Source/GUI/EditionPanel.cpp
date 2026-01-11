#include "EditionPanel.h"
#include "juce_graphics/juce_graphics.h"

//==============================================================================
EditionPanel::EditionPanel() {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

EditionPanel::~EditionPanel() {}

void EditionPanel::paint(juce::Graphics& g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.fillAll(juce::Colours::navy);
    g.setColour(juce::Colours::yellow);
    g.setFont(20.0f);
    g.drawText("Edition Palette", getLocalBounds(),
               juce::Justification::centred, true);
}

void EditionPanel::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..
}
