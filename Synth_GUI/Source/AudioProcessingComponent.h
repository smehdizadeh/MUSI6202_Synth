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
    void SetFrq(double);
    void GetKey(int);
    void ToggleSynth();

    enum Source
    {
        karplus,
        square,
        triangle,
        numSources
    };

    enum Note
    {
        c = 90,
        cs = 83,
        d = 88,
        ds = 68,
        e = 67,
        f = 86,
        fs = 71,
        g = 66,
        gs = 72,
        a = 78,
        as = 74,
        b = 77,
        cOct = 44,
        numNotes = 13
    };

    void NextSource();

private:
    //=========================================================================
    float* m_pfSoundArray;
    double m_dFreq;
    float m_fSampleRate;
    int m_fSampExpect;
    int m_iNumChannels;
    int m_iNumKeysDown; // used for synth I/O logic
    bool m_bPlaying;
    Source m_kSource;
    Note m_kNote;

    juce::AudioBuffer<float> audioBuffer; //for temporary storage and processing
    juce::KeyPress key; //UI keyboard presses
    juce::ADSR env; //envelope to apply to sound gen

    // Modules
    FilterComponent* filt;
    KarplusStrong* KS;
    ReverbComponent* revrb;

    Additive* Add;
    double m_dWaveSamp;
    double m_dTime;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessingComponent)
};