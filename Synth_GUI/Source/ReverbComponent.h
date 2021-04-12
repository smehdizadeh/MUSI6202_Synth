/*
  ==============================================================================

    ReverbComponent.h
    Created: 11 Apr 2021 8:55:26pm
    Author:  sophi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ReverbComponent  : public juce::Component
{
public:
    //==========================================================================
    ReverbComponent() :
        forwardFFT(fftOrder)
    {
        

    }

    ~ReverbComponent() override
    {
    }

    //==========================================================================
    static constexpr auto fftOrder = 10; //represents size of FFT (2^fftOrder)

private:
    //==========================================================================
    juce::dsp::FFT forwardFFT;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbComponent)
};
