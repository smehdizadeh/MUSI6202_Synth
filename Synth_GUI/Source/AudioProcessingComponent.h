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
    void setSampleRate(float newSampRate); //called by GUIComponent when user changes samp rate

private:
    //=========================================================================
    void changeSampleRate(float* pfAudio, int numSamples); //called within APC during getNextAudioBlock to change the samp rate at the output

    //=========================================================================
    float* m_pfSoundArray;
    float m_fFreq;
    float m_fSampleRate; //internal sample rate
    float m_fOutputSampRate; //output sample rate

    int m_fSampExpect;
    int m_iNumChannels;
    Source m_kSource;

    juce::AudioBuffer<float> audioBuffer; //for temporary storage and processing
    juce::KeyPress key; //UI keyboard presses
    juce::ADSR env; //envelope to apply to sound gen
    juce::IIRFilter antiAlias; //anti aliasing filter for downsampling

    // Modules
    FilterComponent* filt;
    KarplusStrong* KS;
    ReverbComponent* revrb;

    Additive* Add;
    double m_dWaveSamp;
    double m_dTime;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessingComponent)
};