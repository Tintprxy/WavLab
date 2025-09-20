#include "MainComponent.h"

// TransportEngine implementation inlined here to avoid include path issues
class TransportEngine : public juce::AudioSource
{
public:
    TransportEngine() : thread ("audio read")
    {
        formatManager.registerBasicFormats();
        thread.startThread();
    }

    bool loadFile (const juce::File& f)
    {
        auto* reader = formatManager.createReaderFor (f);
        if (reader == nullptr) return false;

        readerSource.reset (new juce::AudioFormatReaderSource (reader, true));
        transport.setSource (readerSource.get(), 4096, &thread, reader->sampleRate);
        transport.setPosition (0.0);
        return true;
    }

    void play()                      { transport.start();  }
    void stop()                      { transport.stop();   }
    void setPosition (double sec)    { transport.setPosition (sec); }
    double getPosition() const       { return transport.getCurrentPosition(); }
    double getLength()   const       { return transport.getLengthInSeconds(); }
    void  setGain (float g)          { transport.setGain (g); }

    // AudioSource
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    { transport.prepareToPlay (samplesPerBlockExpected, sampleRate); }

    void releaseResources() override
    { transport.releaseResources(); }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& info) override
    { transport.getNextAudioBlock (info); }

private:
    juce::AudioFormatManager formatManager;
    juce::TimeSliceThread    thread;
    juce::AudioTransportSource transport;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
};


MainComponent::MainComponent()
{
    setSize (1000, 640);

    // create engine
    engine = new TransportEngine();

    // initialize audio device and engine
    deviceManager.initialise (0, 2, nullptr, true);
    audioPlayer.setSource (engine);
    deviceManager.addAudioCallback (&audioPlayer);

    // Hook callbacks
    topBar.onPlay = [this]
    {
        // if nothing loaded yet, prompt user
        if (engine->getLength() <= 0.0)
        {
            // Use async file chooser so we don't block the message thread. Keep the FileChooser alive
            // until the callback runs by storing it in a shared_ptr and capturing it in the lambda.
            auto fc = std::make_shared<juce::FileChooser> ("Open audio", juce::File{}, "*.wav;*.mp3;*.aiff");
            fc->launchAsync (juce::FileBrowserComponent::openMode
                             | juce::FileBrowserComponent::canSelectFiles,
                             [this, fc] (const juce::FileChooser& chooser)
                             {
                                 auto file = chooser.getResult();
                                 if (file.existsAsFile())
                                 {
                                     engine->loadFile (file);
                                     engine->play();
                                     isPlaying = true;
                                     content.setPlaying (true);
                                 }
                             });
            // IMPORTANT: after launchAsync, return from onPlay so you don't fall through
            return;
        }

        // file already loaded - just start playback
        engine->play();
        isPlaying = true;
        content.setPlaying (true);
    };
    topBar.onStop = [this]
    {
        engine->stop();
        engine->setPosition (0.0);
        isPlaying = false;
        content.setPlaying (false);
        content.resetPlayhead();
    };
    topBar.onTempoChanged = [this] (double bpm)
    {
        content.setTempo (bpm);
    };
    topBar.onTimeSigChanged = [this] (int n, int d)
    {
        content.setTimeSig (n, d);
    };

    addAndMakeVisible (topBar);
    addAndMakeVisible (sideBar);
    addAndMakeVisible (content);

    // initialize defaults
    content.setTempo (120.0);
    content.setTimeSig (4, 4);

    // let content follow engine time when playing
    content.setExternalTimeProvider ([this]{ return engine->getPosition(); });

    // allow scrubbing to set engine position too
    content.setExternalScrubCallback ([this](double sec) { engine->setPosition (sec); });

    // keyboard focus for spacebar transport
    setWantsKeyboardFocus (true);
    juce::MessageManager::callAsync ([this] { grabKeyboardFocus(); });
}

MainComponent::~MainComponent()
{
    deviceManager.removeAudioCallback (&audioPlayer);
    audioPlayer.setSource (nullptr);
    delete engine;
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

    content.setBounds (bounds);
}

bool MainComponent::keyPressed (const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        if (isPlaying) { if (topBar.onStop) topBar.onStop(); }
        else           { if (topBar.onPlay) topBar.onPlay(); }
        return true;
    }
    return false;
}
