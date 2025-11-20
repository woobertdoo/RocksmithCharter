/*
  ==============================================================================

    MainComponent.cpp
    Created: 23 Apr 2025 10:11:22pm
    Author:  jamieharris

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(400, 300);
}

MainComponent::~MainComponent() {}

void MainComponent::paint(juce::Graphics& g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.fillAll(juce::Colours::lightblue);
    g.setColour(juce::Colours::darkblue);
    g.setFont(20.0f);
    g.drawText(currentSizeAsString, getLocalBounds(),
               juce::Justification::centred, true);
}

void MainComponent::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..
    currentSizeAsString =
        juce::String(getWidth()) + " x " + juce::String(getHeight());
}
