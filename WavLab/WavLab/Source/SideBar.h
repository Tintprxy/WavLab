#pragma once
#include <JuceHeader.h>

class SideBar : public juce::Component
{
public:
    SideBar() { setName ("SideBar"); }
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour::fromRGB (28, 30, 35));
        g.setColour (juce::Colours::white.withAlpha (0.6f));
#ifdef _MSC_VER
#pragma warning(suppress:4996)
#endif
        g.setFont (juce::Font (15.0f));
        g.drawText ("Side", getLocalBounds(), juce::Justification::centred, true);
    }
};