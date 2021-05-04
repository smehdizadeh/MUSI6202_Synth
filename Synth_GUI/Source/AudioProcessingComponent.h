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

    //Parameters to set via the GUI
    void setFrq(double); //Set frequency of synthesizer pitch
    void setTranspositionVal(double); //Set Octave
    void setPlaying(bool); //Turn synth on or off
    void toggleReverb(); //Toggles reverb
    void setSampleRate(float newSampRate); //called by GUIComponent when user changes samp rate
    void setBitDepth(float newBitDepth); //called by GUIComponent when user changes bit depth
    void setDither(bool enableDither); //called by GUIComponent when user toggles dither button
    void setSource(int); //called by GUIComponent when user changes synth sound source/osc
    void setNumHarmonics(float); //Set number of harmonics for additive synthesis
    void setLPFCutoff(float); //Set the LPF cutoff frequency
    void setCombFilterVal(int);
    void setFlangerFrq(float);
    void setChorusFrq(float);
    void setVibratoFrq(float);

    enum class Source
    {
        karplus,
        sine, 
        square,
        triangle,
        numSources
    };

    enum class Effects
    {
        none,
        reverb,
        lpf,
        comb,
        flanger,
        chorus,
        vibrato,
        numEffects
    };

    //signal pipeline variables (needs to be public for GUI component implementation)
    Effects* effects;
    void setEffect(Effects&, int);
    void ModuleManager(Effects*, juce::AudioBuffer<float>&, int, int, float*, float, float);
    

private:
    //=========================================================================
    void changeSampleRate(float* pfAudio, int numSamples); //called within APC during getNextAudioBlock to change the samp rate at the output
    void changeBitDepth(float* pfAudio, int numSamples); //called within APC during getNextAudioBlock to change the bit depth at the output
    void applyReverb(juce::AudioBuffer<float>&, int, int);
    void applyMovingAverageFilter(float*, int, float, float);
    void applyCombFilter();
    void applyFlanger();
    void applyChorus();
    void applyVibrato();

    //=========================================================================
    float* m_pfSoundArray;
    int m_iNumChannels;

    //Sound Generation variables
    double m_dWaveSamp; //Wave samples
    int m_iNumKeysDown; // used for synth I/O logic
    Source m_kSource; // Wave gen
    double m_dFreq; // Frequency of note
    double m_dTransposeVal; //Octave
    bool m_bPlaying; // Is the synth playing?
    float m_fNumHarmonics; //Number of harmonics in additive synthesis
    
    //Effects variables
    bool m_bReverbOn; //Is the reverb on?
    bool m_bDitherOn; //Is dither enabled?
    float m_fLpfCutoff; //Cutoff frequency of the LPF
    int m_iCombFilterVal; //Comb filter: number of delayed samples
    float m_fFlangerFrq; //Flanger frequency
    float m_fChorusFrq; //Chorus frequency
    float m_fVibratoFrq; // Vibrato freqency

    //Sample rate variables
    float m_fSampleRate; //internal sample rate
    float m_fOutputSampRate; //output sample rate
    float m_fOutputBitDepth; //output bit depth

    juce::AudioBuffer<float> audioBuffer; //for temporary storage and processings
    juce::ADSR env; //envelope to apply to sound gen
    juce::IIRFilter antiAlias; //anti aliasing filter for downsampling
    juce::Random random; //noise source for dither
    juce::AudioDeviceManager manager;
    juce::AudioDeviceManager::AudioDeviceSetup audioSetup;

    // Modules
    FilterComponent* filt;
    KarplusStrong* KS;
    ReverbComponent* revrb;
    Additive* Add;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessingComponent)
};