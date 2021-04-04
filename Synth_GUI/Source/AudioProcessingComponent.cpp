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

    setAudioChannels(0, 2); // no inputs, two outputs
    FS = deviceManager.getAudioDeviceSetup().sampleRate;
    KS = new KarplusStrong(0.9, 200, FS);
    writeIdx = 0;
    soundArray = new float[FS];
    KS->CreateOutput();
    KS->GetKarpArray(soundArray);
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    //delete[] KS;
    delete[] soundArray;
    shutdownAudio();
    FS = 0;
    writeIdx = 0;
    
    
}


//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::String message;
    message << "Preparing to play audio...\n";
    message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
    message << " sampleRate = " << sampleRate;
    juce::Logger::getCurrentLogger()->writeToLog(message);
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
            buffer[sample] = soundArray[KS->GetKarpWriteIdx()];
            KS->SetKarpWriteIdx();
            //buffer[sample] = random.nextFloat() * 0.25f - 0.125f;
        }
    }
}

void AudioProcessingComponent::releaseResources()
{

}