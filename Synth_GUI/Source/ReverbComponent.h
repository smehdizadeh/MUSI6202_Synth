/*
  ==============================================================================

    ReverbComponent.h
    Created: 11 Apr 2021 8:55:26pm
    Author:  sophi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <complex>
#include "impulseAssets.h"

//==============================================================================
/*
*/
class ReverbComponent  : public juce::MPESynthesiser
{
public:
    //==========================================================================
    ReverbComponent(float fSampleRate, int iBlockSize) :
        m_fSampleRate(fSampleRate),
        m_iBlockSize(iBlockSize)
    {
        auto& conv = fxChain.get<convolutionIndex>(); //access the convolution inside the processor chain
        //load the impulse response from assets (binary)
        //conv.loadImpulseResponse(impulseAssets::impulseresponse_wav, (size_t)impulseAssets::impulseresponse_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::no, (size_t)impulseAssets::impulseresponse_wavSize);
        conv.loadImpulseResponse(impulseAssets::RoomHuge_wav, (size_t)impulseAssets::RoomHuge_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::no, (size_t)impulseAssets::RoomHuge_wavSize);

        //specs for convolution
        spec.sampleRate = m_fSampleRate;
        spec.numChannels = 1;
        spec.maximumBlockSize = m_iBlockSize;

        prepare(spec);        
    }

    ~ReverbComponent() override
    {
        fxChain.reset();

        m_fSampleRate = 0;
        m_iBlockSize = 0;
    }

    //==========================================================================
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept
    {
        fxChain.prepare(spec); // Must call this before rendering audio block
    }

    void renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {
        auto block = juce::dsp::AudioBlock<float>(outputAudio).getSubBlock((size_t)startSample, (size_t)numSamples);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        fxChain.process(context);
    }

private:
    //==========================================================================
    enum
    {
        convolutionIndex
    };

    juce::dsp::ProcessorChain < juce::dsp::Convolution > fxChain; //houses the convolution.. not sure why juce makes you do this...
    juce::dsp::ProcessSpec spec; //specs for the convolution

    int m_iBlockSize; //size of each new incoming audio block from APC
    float m_fSampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbComponent)
};
