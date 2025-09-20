#pragma once
#include <JuceHeader.h>

class TopBar : public juce::Component
{
public:
    TopBar() { setName ("TopBar"); }
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour::fromRGB (22, 22, 28));          // bg
        g.setColour (juce::Colours::white.withAlpha (0.8f));
#ifdef _MSC_VER
#pragma warning(suppress:4996)
#endif
        g.setFont (juce::Font (16.0f, juce::Font::bold));
        g.drawText ("TopBar", getLocalBounds().reduced (12, 0),
                    juce::Justification::centredLeft, true);
    }
};