/*
  ==============================================================================

    AudioProcessingComponent.cpp
    Created: 30 Mar 2021 6:08:33pm
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
    
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();
}

void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

}

void AudioProcessingComponent::releaseResources()
{

}

void AudioProcessingComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{

}