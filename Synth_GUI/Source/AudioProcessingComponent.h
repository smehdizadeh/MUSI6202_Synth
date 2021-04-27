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
#include "ReverbComponent.h"

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

    enum Source;
    void ChangeSource(Source source);

private:
    //=========================================================================
    float* m_pfSoundArray;
    float m_fFreq;
    float m_fSampleRate;
    int m_fSampExpect;
    int m_iNumChannels;
    Source m_kSource;

    juce::AudioBuffer<float> audioBuffer; //for temporary storage and processing
    juce::KeyPress key; //UI keyboard presses
    juce::ADSR env; //envelope to apply to sound gen

    // Modules
    FilterComponent* filt;
    KarplusStrong* KS;
    ReverbComponent* revrb;

    juce::KeyPress key;
    Additive* Add;
    double m_dWaveSamp;
    double m_dTime;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessingComponent)
};