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
AudioProcessingComponent::AudioProcessingComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    m_iNumChannels = 2;
    m_fSampleRate = deviceManager.getAudioDeviceSetup().sampleRate;

    filt = new FilterComponent(m_fSampleRate, m_iNumChannels);

    setAudioChannels(0, m_iNumChannels); // no inputs, two outputs
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();
    audioBuffer.clear();

    delete filt;
}

//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audioBuffer.setSize(1, samplesPerBlockExpected); //mono working buffer
    audioBuffer.clear();
}

void AudioProcessingComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // EVERYTHING DONE IN MONO AND THEN COPIED TO ADDITIONAL CHANNELS
    auto* p = audioBuffer.getWritePointer(0); //pointer to working buffer

    // Fill the required number of samples with noise between -0.125 and +0.125
    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        p[sample] = random.nextFloat() * 0.25f - 0.125f;

    filt->processIIRLPF(p, p, bufferToFill.numSamples, 0.0); // LP filter noise

    // send to the Juce output buffer (ALL CHANNELS)
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, p, bufferToFill.numSamples);
    }
}

void AudioProcessingComponent::releaseResources()
{

}