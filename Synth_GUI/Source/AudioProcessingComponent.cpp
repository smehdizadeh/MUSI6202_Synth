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
    KS(0),
    Add(0),
    m_pfSoundArray(0),
    m_fFreq(0),
    m_fSampleRate(0),
    m_iNumChannels(2),
    filt(0),
    key(0),
    m_dWaveSamp(0),
    m_dTime(0),
    m_fSampExpect(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setAudioChannels(0, m_iNumChannels); // no inputs, two outputs
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();  
    delete[] m_pfSoundArray;
    KS->~KarplusStrong();
    Add->~Additive();
    audioBuffer.clear();
    filt = 0;
}


//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_fSampleRate = sampleRate;
    m_fSampExpect = samplesPerBlockExpected;
    filt = new FilterComponent(m_fSampleRate);
    // Code for Karplus Strong Algorithm

    m_fFreq = 440;
    KS = new KarplusStrong(m_fSampleRate);
    KS->CreateOutput();
    m_pfSoundArray = new float[m_fSampleRate];
    KS->GetKarpArray(m_pfSoundArray);

    Add = new Additive(0);
    // End code for Karplus Strong Algorithm
  
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
            //AdditiveBlock
            Add->GetSample(m_dWaveSamp, m_dTime, m_fSampleRate, 1, 220, 5); // 3rd and four
            p[sample] = m_dWaveSamp;

            //KarplusBlock
            //p[sample] = m_pfSoundArray[KS->GetKarpWriteIdx()];
        }
        else
        {
            p[sample] = 0;
        }
        KS->SetKarpWriteIdx((int)(m_fSampleRate/m_fFreq));
    }

    // CUTOFF RANGE IS 22 Hz - 20 kHz, GAIN RANGE IS 0.0 - 1.0
    filt->processMovingAvgFilt(p, p, bufferToFill.numSamples, filt->GetCutoffFreq(), 0.9); //LP Filter noise

    // send to the Juce output buffer (ALL CHANNELS)
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, p, bufferToFill.numSamples);
    }
}

void AudioProcessingComponent::releaseResources()
{

}

//enum Source
//{
//    additive,
//    karplus
//};
//
//void ChangeSource(Source source)
//{
//    m_kSource = source;
//}