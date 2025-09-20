#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (900, 600);

    addAndMakeVisible (topBar);
    addAndMakeVisible (sideBar);
    addAndMakeVisible (content);
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();

    auto top = bounds.removeFromTop (kTopBarHeight);
    topBar.setBounds (top);

    auto left = bounds.removeFromLeft (kSideBarWidth);
    sideBar.setBounds (left);

    content.setBounds (bounds); // whatever space is left
}
