#pragma once
#include <JuceHeader.h>

// Audio modules not included by default JuceHeader in this project - include the specific JUCE audio modules used here
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_utils/juce_audio_utils.h>

#include "TopBar.h"
#include "SideBar.h"
#include "ContentArea.h"

class TransportEngine; // forward declaration (defined in TransportEngine.h included in .cpp)

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override; // defined in cpp

    void paint (juce::Graphics&) override;
    void resized() override;
    bool keyPressed (const juce::KeyPress&) override;

private:
    TopBar      topBar;
    SideBar     sideBar;
    ContentArea content;

    // audio
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer  audioPlayer;
    TransportEngine*         engine = nullptr; // will be created in cpp

    // layout constants
    static constexpr int kTopBarHeight = 56;
    static constexpr int kSideBarWidth = 200;

    // transport state
    bool isPlaying = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
