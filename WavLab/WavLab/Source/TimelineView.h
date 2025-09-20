#pragma once
#include <JuceHeader.h>

struct TimelineConfig
{
    double bpm        = 120.0; // beats per minute
    int    numer      = 4;     // time signature numerator
    int    denom      = 4;     // time signature denominator
    double playhead   = 0.0;   // seconds
    double pixelsPerSecond = 120.0; // zoom level
};

class TimelineView : public juce::Component
{
public:
    // input callbacks
    std::function<void(double)> onScrubToSeconds;
    std::function<void(double)> onZoomPixelsPerSecond;

    TimelineView() { setInterceptsMouseClicks (true, true); }

    void setConfig (const TimelineConfig& c) { config = c; repaint(); }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour::fromRGB (18,18,20));

        auto r = getLocalBounds().toFloat();
        const double sPerBeat = 60.0 / config.bpm;
        const double sPerBar  = sPerBeat * config.numer;
        const float  pxPerSec = (float) config.pixelsPerSecond;

        // draw bar + beat grid
        g.setColour (juce::Colour::fromRGB (40,42,48));
        for (float x = 0.0f; x < r.getWidth(); x += pxPerSec)        // seconds grid (light)
            g.drawVerticalLine ((int) std::round (x), 0.0f, r.getHeight());

        g.setColour (juce::Colour::fromRGB (58,61,70));
        for (double t = 0.0; (t * pxPerSec) < r.getWidth(); t += sPerBeat) // beat grid
            g.drawVerticalLine ((int) std::round (t * pxPerSec), 0.0f, r.getHeight());

        g.setColour (juce::Colour::fromRGB (85,90,102));             // bar grid (stronger)
        for (double t = 0.0; (t * pxPerSec) < r.getWidth(); t += sPerBar)
            g.drawVerticalLine ((int) std::round (t * pxPerSec), 0.0f, r.getHeight());

        // playhead
        const float xPlay = (float) (config.playhead * pxPerSec);
        g.setColour (juce::Colours::white);
        g.drawLine (xPlay, 0.0f, xPlay, r.getHeight(), 2.0f);

        // header text
        g.setColour (juce::Colours::white.withAlpha (0.6f));
#ifdef _MSC_VER
#pragma warning(suppress:4996)
#endif
        g.setFont (14.0f);
        g.drawText (juce::String (config.bpm, 1) + " bpm  •  "
                    + juce::String (config.numer) + "/" + juce::String (config.denom),
                    r.removeFromTop (20).toNearestInt(), juce::Justification::centredLeft, true);
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        if (onScrubToSeconds)
            onScrubToSeconds (e.position.x / (double) config.pixelsPerSecond);
    }
    void mouseDrag (const juce::MouseEvent& e) override { mouseDown (e); }

    void mouseWheelMove (const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override
    {
        if (e.mods.isCtrlDown())
        {
            const double pps = juce::jlimit (30.0, 600.0,
                                  config.pixelsPerSecond * (1.0 + wheel.deltaY * 0.20));
            if (onZoomPixelsPerSecond) onZoomPixelsPerSecond (pps);
        }
        else
        {
            Component::mouseWheelMove (e, wheel);
        }
    }

private:
    TimelineConfig config;
};
