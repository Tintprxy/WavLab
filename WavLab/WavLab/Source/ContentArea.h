#pragma once
#include <JuceHeader.h>
#include "TimelineView.h"
#include <functional>

class ContentArea : public juce::Component, private juce::Timer
{
public:
    ContentArea()
    {
        addAndMakeVisible (timeline);

        // wire input callbacks from timeline
        timeline.onScrubToSeconds = [this](double sec)
        {
            cfg.playhead = std::max (0.0, sec);
            playing = false;                    // stop when scrubbing
            if (externalScrubCallback)
                externalScrubCallback (cfg.playhead);
            timeline.setConfig (cfg);
        };

        timeline.onZoomPixelsPerSecond = [this](double pps)
        {
            cfg.pixelsPerSecond = pps;
            timeline.setConfig (cfg);
        };

        startTimerHz (60); // 60 FPS playhead updates
    }

    // state setters used by MainComponent
    void setTempo (double bpm)            { cfg.bpm = bpm;      timeline.setConfig (cfg); }
    void setTimeSig (int n, int d)        { cfg.numer = n; cfg.denom = d; timeline.setConfig (cfg); }
    void setPlaying (bool isPlaying)      { playing = isPlaying; }
    void resetPlayhead()                  { cfg.playhead = 0.0; timeline.setConfig (cfg); }

    void setExternalTimeProvider (std::function<double()> fn) { timeProvider = std::move (fn); }
    void setExternalScrubCallback (std::function<void(double)> fn) { externalScrubCallback = std::move (fn); }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour::fromRGB (12,12,14));
    }

    void resized() override
    {
        timeline.setBounds (getLocalBounds().reduced (8, 8));
    }

private:
    void timerCallback() override
    {
        if (playing)
        {
            if (timeProvider)
                cfg.playhead = timeProvider();
            else
                cfg.playhead += 1.0 / 60.0;

            timeline.setConfig (cfg);
        }
    }

    TimelineConfig cfg;
    bool playing { false };
    TimelineView timeline;
    std::function<double()> timeProvider;
    std::function<void(double)> externalScrubCallback;
};
