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
    m_pfSoundArray(0),
    m_fFreq(0),
    m_dFS(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setAudioChannels(0, 2); // no inputs, two outputs
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();  
    delete[] m_pfSoundArray;
    KS->~KarplusStrong();
}


//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Code for Karplus Strong Algorithm

    m_fFreq = 440;
    KS = new KarplusStrong(sampleRate);
    KS->CreateOutput();
    m_pfSoundArray = new float[sampleRate];
    KS->GetKarpArray(m_pfSoundArray);
    m_dFS = sampleRate;

    // End code for Karplus Strong Algorithm


}



void AudioProcessingComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        // Get a pointer to the start sample in the buffer for this audio output channel
        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

        // Fill the required number of samples with noise between -0.125 and +0.125
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            buffer[sample] = m_pfSoundArray[KS->GetKarpWriteIdx()];
            KS->SetKarpWriteIdx((int)(m_dFS/m_fFreq));
        }
    }
}

void AudioProcessingComponent::releaseResources()
{

}