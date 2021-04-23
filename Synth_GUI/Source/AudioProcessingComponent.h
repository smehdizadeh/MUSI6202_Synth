/*
  ==============================================================================

    AudioProcessingComponent.h
    Created: 31 Mar 2021 11:52:36am
    Author:  sophi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterComponent.h"
#include "ModEffectsComponent.h"

//==============================================================================
/*
*/
class AudioProcessingComponent  : public juce::AudioAppComponent
{
public:
    //==========================================================================
    AudioProcessingComponent();
    ~AudioProcessingComponent() override;

    //==========================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
    //=========================================================================
    float m_fSampleRate;
    int m_iNumChannels;
    juce::AudioBuffer<float> audioBuffer; //for temporary storage and processing
    juce::Random random; //for noise synthesis
    // define the adsr juce class
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    // Modules
    FilterComponent* filt;
    ModEffectsComponent* mod;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessingComponent)
};
