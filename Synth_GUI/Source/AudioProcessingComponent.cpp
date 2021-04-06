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
    m_fSampleRate(0),
    m_iNumChannels(2),
    filt(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setAudioChannels(0, m_iNumChannels); // no inputs, two outputs
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();
    audioBuffer.clear();
    filt = 0;
}

//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audioBuffer.setSize(1, samplesPerBlockExpected); //mono working buffer
    audioBuffer.clear();

    m_fSampleRate = sampleRate;
    filt = new FilterComponent(m_fSampleRate);
}

void AudioProcessingComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // EVERYTHING DONE IN MONO AND THEN COPIED TO ADDITIONAL CHANNELS
    auto* p = audioBuffer.getWritePointer(0); //pointer to working buffer

    // Fill the required number of samples with noise between -0.125 and +0.125
    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        p[sample] = random.nextFloat() * 0.25f - 0.125f;

    // CUTOFF RANGE IS 22 Hz - 20 kHz, GAIN RANGE IS 0.0 - 1.0
    filt->processMovingAvgFilt(p, p, bufferToFill.numSamples, 1000.0, 0.9); //LP Filter noise

    // send to the Juce output buffer (ALL CHANNELS)
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, p, bufferToFill.numSamples);
    }
}

void AudioProcessingComponent::releaseResources()
{

}