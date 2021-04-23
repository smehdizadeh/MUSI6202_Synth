/*
  ==============================================================================

    AudioProcessingComponent.cpp
    Created: 31 Mar 2021 11:52:36am
    Author:  sophi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioProcessingComponent.h"


//==============================================================================
AudioProcessingComponent::AudioProcessingComponent() :
    m_pfSoundArray(0),
    m_fFreq(0),
    m_fSampleRate(0),
    m_iNumChannels(2),
    KS(0),
    filt(0),
    revrb(0),
    key(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setAudioChannels(0, m_iNumChannels); // no inputs, two outputs
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();
    audioBuffer.clear();

    delete[] m_pfSoundArray;

    KS->~KarplusStrong();
    KS = 0;
    filt->~FilterComponent();
    filt = 0;
    revrb->~ReverbComponent();
    revrb = 0;
}


//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_fSampleRate = sampleRate;
    filt = new FilterComponent(m_fSampleRate); //create filter module
    revrb = new ReverbComponent(m_fSampleRate, samplesPerBlockExpected); //create reverb module

    // Code for Karplus Strong Algorithm
    m_fFreq = 440; //KS frequency
    KS = new KarplusStrong(m_fSampleRate); //create KS generator
    KS->CreateOutput();
    m_pfSoundArray = new float[m_fSampleRate]; //KS buffer
    KS->GetKarpArray(m_pfSoundArray);

  
    audioBuffer.setSize(1, samplesPerBlockExpected); //mono working buffer
    audioBuffer.clear();
}



void AudioProcessingComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // EVERYTHING DONE IN MONO AND THEN COPIED TO ADDITIONAL CHANNELS
    if (key.isKeyCurrentlyDown(juce::KeyPress::upKey))
    {
        filt->AdjustCutoffFreq(juce::KeyPress::upKey);
    }

    if (key.isKeyCurrentlyDown(juce::KeyPress::downKey))
    {
        filt->AdjustCutoffFreq(juce::KeyPress::downKey);
    }

    auto* p = audioBuffer.getWritePointer(0); //pointer to working buffer


    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        if (key.isKeyCurrentlyDown(juce::KeyPress::spaceKey))
        {
            p[sample] = m_pfSoundArray[KS->GetKarpWriteIdx()];
        }
        else
        {
            p[sample] = 0;
        }
        KS->SetKarpWriteIdx((int)(m_fSampleRate/m_fFreq));
    }

    // CUTOFF RANGE IS 22 Hz - 20 kHz, GAIN RANGE IS 0.0 - 1.0
    filt->processMovingAvgFilt(p, p, bufferToFill.numSamples, filt->GetCutoffFreq(), 0.9); //LP Filter

    // CONVOLUTIONAL REVERB TESTING
    revrb->processConvReverb(p, p, bufferToFill.numSamples);

    // send to the Juce output buffer (ALL CHANNELS)
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, p, bufferToFill.numSamples);
    }
}

void AudioProcessingComponent::releaseResources()
{

}