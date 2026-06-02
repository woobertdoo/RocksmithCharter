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
    timelinePanel = new TimelinePanel();
    addAndMakeVisible(timelinePanel);

    editionPanel = new EditionPanel();
    addAndMakeVisible(editionPanel);

    previewPanel = new PreviewPanel();
    addAndMakeVisible(previewPanel);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(1600, 900);
}

MainComponent::~MainComponent() {
    delete timelinePanel;
    delete editionPanel;
    delete previewPanel;
}

void MainComponent::paint(juce::Graphics& g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.fillAll(juce::Colours::lightblue);
    g.setColour(juce::Colours::darkblue);
    g.setFont(20.0f);
}

void MainComponent::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto area = getLocalBounds();
    int timelinePanelHeight = getHeight() / 2;

    timelinePanel->setBounds(area.removeFromTop(timelinePanelHeight));

    int editionPanelWidth = (getWidth() / 5) * 2;

    editionPanel->setBounds(area.removeFromLeft(editionPanelWidth));

    previewPanel->setBounds(area);
}
