#pragma once
#include <JuceHeader.h>
#include <functional>

class TopBar : public juce::Component
{
public:
    // callbacks into MainComponent
    std::function<void()> onPlay;
    std::function<void()> onStop;
    std::function<void(double)> onTempoChanged;
    std::function<void(int,int)> onTimeSigChanged;

    TopBar()
    {
        setName ("TopBar");

        playBtn.setButtonText ("Play");
        stopBtn.setButtonText ("Stop");

        tempo.setRange (40.0, 240.0, 0.1);
        tempo.setValue (120.0);
        tempo.setTextValueSuffix (" bpm");

        numer.addItem ("2", 1); numer.addItem ("3", 2);
        numer.addItem ("4", 3); numer.addItem ("5", 4); numer.addItem ("6", 5);
        numer.setSelectedId (3); // 4

        denom.addItem ("4", 1); denom.addItem ("8", 2);
        denom.setSelectedId (1); // 4

        addAndMakeVisible (playBtn);
        addAndMakeVisible (stopBtn);
        addAndMakeVisible (tempo);
        addAndMakeVisible (numer);
        addAndMakeVisible (denom);

        playBtn.onClick = [this]{ if (onPlay) onPlay(); };
        stopBtn.onClick = [this]{ if (onStop) onStop(); };
        tempo.onValueChange = [this]{ if (onTempoChanged) onTempoChanged (tempo.getValue()); };
        auto pushTS = [this]
        {
            if (onTimeSigChanged)
                onTimeSigChanged (numer.getText().getIntValue(), denom.getText().getIntValue());
        };
        numer.onChange = pushTS;
        denom.onChange = pushTS;
    }

    void paint (juce::Graphics& g) override
    {
        // background only - no text drawn here per request
        g.fillAll (juce::Colour::fromRGB (22,22,28));
        g.setColour (juce::Colours::white.withAlpha (0.8f));
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (8, 6);
        playBtn.setBounds (r.removeFromLeft (70));
        r.removeFromLeft (6);
        stopBtn.setBounds (r.removeFromLeft (70));
        r.removeFromLeft (12);

        tempo.setBounds (r.removeFromLeft (160));
        r.removeFromLeft (12);

        numer.setBounds (r.removeFromLeft (60));
        r.removeFromLeft (6);
        denom.setBounds (r.removeFromLeft (60));
    }

private:
    juce::TextButton playBtn, stopBtn;
    juce::Slider tempo { juce::Slider::LinearBar, juce::Slider::TextBoxRight };
    juce::ComboBox numer, denom;
};
