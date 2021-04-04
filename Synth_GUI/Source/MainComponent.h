#pragma once

#include <JuceHeader.h>
#include "GUIComponent.h"
#include "AudioProcessingComponent.h"

//==============================================================================
/*
    This component lives inside our window.
    Creates two branches, one to the GUI/visual processing, one to the audio processing
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    GUIComponent* gui;
    AudioProcessingComponent apc;
    juce::AudioSampleBuffer karplusTable;
    juce::OwnedArray<KarplusStrong> oscillators;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
