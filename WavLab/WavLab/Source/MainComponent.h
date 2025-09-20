#pragma once
#include <JuceHeader.h>
#include "TopBar.h"
#include "SideBar.h"
#include "ContentArea.h"

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TopBar      topBar;
    SideBar     sideBar;
    ContentArea content;

    // layout constants (tweak later / make user-configurable)
    static constexpr int kTopBarHeight  = 48;
    static constexpr int kSideBarWidth  = 200;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
