/*
  ==============================================================================

    FilterComponent.h
    Created: 2 Apr 2021 2:42:04pm
    Author:  sophi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RingBuffer.h"

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    //==========================================================================
    FilterComponent(float fSampleRate, int iNumChannels) :
        m_fSampleRate(fSampleRate),
        m_iNumChannels(iNumChannels),
        m_iDelayInSamples(0),
        m_fAlpha(0),
        m_iBufSize(50) //arbitrary
    {
        buf = new RingBuffer(m_iBufSize);
    }

    ~FilterComponent() override
    {
        m_fSampleRate = 0;
        m_iNumChannels = 0;
        m_iDelayInSamples = 0;
        m_fAlpha = 0;

        delete buf;
        m_iBufSize = 0;
    }

    //=========================================================================
    void processIIRLPF(float* ppfInputBuffer, float* ppfOutputBuffer, int iBufferSize, float fCutoff)
    {
        m_fAlpha = fCutoff; //how does this relate to cutoff?
        for (int i = 0; i < iBufferSize; i++)
        {
            float val = (1-m_fAlpha) * ppfInputBuffer[i];
            buf->pushSample(val);
            ppfOutputBuffer[i] = val + (m_fAlpha * buf->getDelayed(1));
        }
    }
    

private:
    //=========================================================================
    float m_fSampleRate; //from APC
    int m_iNumChannels; //from APC

    int m_iDelayInSamples; //for Comb Filter...from user GUI
    float m_fAlpha; //for LPF...from user GUI

    RingBuffer* buf;
    int m_iBufSize;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
