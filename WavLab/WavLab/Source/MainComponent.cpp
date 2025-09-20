#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (400, 300); // window size
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);           // background
    g.setColour (juce::Colours::white);         // text color
    g.setFont (juce::Font (24.0f));             // font size
    g.drawText ("Hello, World!",                // text
                getLocalBounds(),               // bounds
                juce::Justification::centred,   // centered
                true);                          // use ellipses if too long
}

void MainComponent::resized()
{
    // layout child components here if you add any
}
