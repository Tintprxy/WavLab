#pragma once
#include <JuceHeader.h>

class ContentArea : public juce::Component
{
public:
    ContentArea() { setName ("ContentArea"); }
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour::fromRGB (18, 18, 20));
        g.setColour (juce::Colours::white.withAlpha (0.5f));
#ifdef _MSC_VER
#pragma warning(suppress:4996)
#endif
        g.setFont (juce::Font (24.0f));
        g.drawText ("Content", getLocalBounds(), juce::Justification::centred, true);
    }
};
