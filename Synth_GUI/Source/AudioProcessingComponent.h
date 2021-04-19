/*
  ==============================================================================

    AudioProcessingComponent.h
    Created: 31 Mar 2021 11:52:36am
    Author:  sophi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SourceGeneration.h"
#include "FilterComponent.h"

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
    float* m_pfSoundArray;
    float m_fFreq;
    float m_fSampleRate;
    int m_iNumChannels;

    juce::AudioBuffer<float> audioBuffer; //for temporary storage and processing

    // Modules
    FilterComponent* filt;
    KarplusStrong* KS;
    juce::KeyPress key;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessingComponent)
};
