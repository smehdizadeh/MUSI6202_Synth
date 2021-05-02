/*
  ==============================================================================

    AudioProcessingComponent.cpp
    Created: 31 Mar 2021 11:52:36am
    Author:  sophi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioProcessingComponent.h"
#include <cmath>



//==============================================================================
AudioProcessingComponent::AudioProcessingComponent() :
    Add(0),
    m_pfSoundArray(0),
    m_dFreq(0),
    m_fSampleRate(0),
    m_fOutputSampRate(0),
    m_fOutputBitDepth(0),
    m_iNumChannels(2),
    KS(0),
    filt(0),
    m_dWaveSamp(0),
    revrb(0),
    m_bPlaying(false),
    m_iNumKeysDown(0),
    m_fLpfCutoff(3000),
    m_fNumHarmonics(200),
    m_bReverbOn(false),
    m_dTransposeVal(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setAudioChannels(0, m_iNumChannels); // no inputs, two outputs
    m_kSource = Source::square;
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();
    audioBuffer.clear();

    delete[] m_pfSoundArray;

    KS->~KarplusStrong();
    KS = 0;
  
    Add->~Additive();
    Add = 0;

    filt->~FilterComponent();
    filt = 0;
  
    revrb->~ReverbComponent();
    revrb = 0;

    env.reset();
    antiAlias.reset();
}


//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_fSampleRate = sampleRate;
    m_fOutputSampRate = sampleRate; 
    m_fOutputBitDepth = 32.0;

    filt = new FilterComponent(m_fSampleRate); //create filter module
    revrb = new ReverbComponent(m_fSampleRate, samplesPerBlockExpected); //create reverb module

    // Code for Karplus Strong Algorithm 
    KS = new KarplusStrong(m_fSampleRate); //create KS generator
    KS->CreateOutput(); //Create KS random samples
    m_pfSoundArray = new float[m_fSampleRate]; //KS buffer
    KS->GetKarpArray(m_pfSoundArray); // Put random samples into buffer

    Add = new Additive();
  
    //Envelope
    env.setSampleRate(sampleRate);
    juce::ADSR::Parameters params;
    params.attack = 0.1;
    params.decay = 0.1;
    params.sustain = 0.9;
    params.release = 0.5;
    env.setParameters(params);
  
    audioBuffer.setSize(1, samplesPerBlockExpected); //mono working buffer
    audioBuffer.clear();
}



void AudioProcessingComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // ***** EVERYTHING DONE IN MONO AND THEN COPIED TO ADDITIONAL CHANNELS ******

    auto* p = audioBuffer.getWritePointer(0); //pointer to working buffer

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
       
        if (m_kSource == Source::square)
        {
            Add->GetSquareSamp(m_dWaveSamp, m_fSampleRate, 1, m_dFreq, m_fNumHarmonics); 
            p[sample] = m_dWaveSamp;
        }

        else if (m_kSource == Source::karplus)
        {
            p[sample] = m_pfSoundArray[KS->GetKarpWriteIdx()];
        }

        else if (m_kSource == Source::triangle)
        {
            Add->GetTriSamp(m_dWaveSamp, m_fSampleRate, 1, m_dFreq, m_fNumHarmonics); 
            p[sample] = m_dWaveSamp;
        }

        //apply ADSR accordingly
        if (m_bPlaying)
        {
            env.noteOn();
        }
        else
        {
            env.noteOff();
        }

        KS->SetKarpWriteIdx((int)(m_fSampleRate/m_dFreq));
    }
    env.applyEnvelopeToBuffer(audioBuffer, 0, bufferToFill.numSamples);

    // CUTOFF RANGE IS 22 Hz - 20 kHz, GAIN RANGE IS 0.0 - 1.0
    filt->processMovingAvgFilt(p, p, bufferToFill.numSamples, m_fLpfCutoff, 0.9); //LP Filter

    // CONVOLUTIONAL REVERB TESTING.. PRESS TAB TO TOGGLE REVERB ON/OFF
    if (m_bReverbOn)
    {
        revrb->renderNextSubBlock(audioBuffer, 0, bufferToFill.numSamples);
    }

    // send to the Juce output buffer (ALL CHANNELS)
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        //perform downsampling here
        changeSampleRate(p, bufferToFill.numSamples);

        //perform down quantizing here
        changeBitDepth(p, bufferToFill.numSamples);

        //copy to output channels
        bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, p, bufferToFill.numSamples);
    }
}

void AudioProcessingComponent::releaseResources()
{
    
}

void AudioProcessingComponent::SetFrq(double m)
{
    m += m_dTransposeVal;
    m_bPlaying = true;
    m_iNumKeysDown += 1;
    m_dFreq = pow(2, (m - 69) / 12) * 440;
}

void AudioProcessingComponent::SetTranspositionVal(double val)
{
    m_dTransposeVal = val;
}

void AudioProcessingComponent::SetNumHarmonics(float val)
{
    m_fNumHarmonics = val;
}

void AudioProcessingComponent::SetPlaying(bool isPlaying)
{
    m_bPlaying = isPlaying;
}

void AudioProcessingComponent::ToggleReverb()
{
    if (m_bReverbOn)
    {
        m_bReverbOn = false;
    }
    else 
    {
        m_bReverbOn = true;
    }
}

void AudioProcessingComponent::SetLPFCutoff(float val)
{
    m_fLpfCutoff = val;
}

void AudioProcessingComponent::NextSource()
{
    if (m_kSource == Source::square)
    {
        m_kSource = Source::karplus;
    }
    else if (m_kSource == Source::karplus)
    {
        m_kSource = Source::triangle;
    }
    else if (m_kSource == Source::triangle)
    {
        m_kSource = Source::square;
    }
}

void AudioProcessingComponent::setSampleRate(float newSampRate)
{
    m_fOutputSampRate = newSampRate;
}

void AudioProcessingComponent::setBitDepth(float newBitDepth)
{
    m_fOutputBitDepth = newBitDepth;
}

void AudioProcessingComponent::changeSampleRate(float* pfAudio, int numSamples)
{
    //first check for default/no change case  (48k)
    if (m_fOutputSampRate == 48000.0) { return; }

    else
    {
        //apply anti-aliasing filter
        antiAlias.reset();
        antiAlias.setCoefficients(juce::IIRCoefficients::makeLowPass(m_fSampleRate, m_fOutputSampRate * 0.5));
        antiAlias.processSamples(pfAudio, numSamples);

        if (m_fOutputSampRate == 16000.0) //then check for the integer factor case (16k)
        {
            for (int i = 0; i < numSamples; i += 3) //take every third sample
            {

            }
        }
        else //rational factor
        {

        }
    }
}

void AudioProcessingComponent::changeBitDepth(float* pfAudio, int numSamples)
{
    //first check for default/no change case  (32 bit / float)
    if (m_fOutputBitDepth == 32.0) { return; }

    else if (m_fOutputBitDepth == 8.0)
    {
        for (int i = 0; i < numSamples; i++)
        {
            pfAudio[i] = static_cast<int8_t>(pfAudio[i]);
        }
    }
}