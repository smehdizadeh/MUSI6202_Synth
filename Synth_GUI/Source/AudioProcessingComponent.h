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
#include <windows.h> //For Debug macro
#include <debugapi.h> //For Debug macro

// Use DBOUT(args) to print to the VS console
#define DBOUT(s)            \
{                             \
std::ostringstream os_;    \
os_ << s;                   \
OutputDebugString(os_.str().c_str()); \
OutputDebugString("\n"); \
}

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

    //called by GUIComponent when user releases a key
    ; //called by GUIComponent when user presses a key
    void SetNumHarmonics(float); //Set number of harmonics for additive synthesis
    void SetPlaying(bool); //Turn synth on or off
    void ToggleReverb();
    void SetFrq(double); //Set frequency of synthesizer pitch

    //Set the octave that the synth plays in
    void SetTranspositionVal(double);

    void SetLPFCutoff(float); //Set the LPF cutoff frequency

    void NextSource(); //called by GUIComponent when user changes synth sound source/osc
    void setSampleRate(float newSampRate); //called by GUIComponent when user changes samp rate
    void setBitDepth(float newBitDepth); //called by GUIComponent when user changes bit depth

    enum Source
    {
        karplus,
        square,
        triangle,
        numSources
    };

private:
    //=========================================================================
    void changeSampleRate(float* pfAudio, int numSamples); //called within APC during getNextAudioBlock to change the samp rate at the output
    void changeBitDepth(float* pfAudio, int numSamples); //called within APC during getNextAudioBlock to change the bit depth at the output

    //=========================================================================
    float* m_pfSoundArray;

    double m_dFreq; // Frequency of note
    int m_iNumKeysDown; // used for synth I/O logic
    bool m_bPlaying; // Is the synth playing?
    float m_fNumHarmonics; //Number of harmonics in additive synthesis
    Source m_kSource; // Wave gen

    float m_fLpfCutoff; //Cutoff frequency of the LPF
    bool m_bReverbOn; //Is the reverb on?

    float m_fSampleRate; //internal sample rate
    float m_fOutputSampRate; //output sample rate
    float m_fOutputBitDepth; //output bit depth

    int m_iNumChannels;

    

    juce::AudioBuffer<float> audioBuffer; //for temporary storage and processings
    juce::ADSR env; //envelope to apply to sound gen
    juce::IIRFilter antiAlias; //anti aliasing filter for downsampling

    // Modules
    FilterComponent* filt;
    KarplusStrong* KS;
    ReverbComponent* revrb;

    Additive* Add;
    double m_dWaveSamp; //Wave samples
    double m_dTransposeVal;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessingComponent)
};