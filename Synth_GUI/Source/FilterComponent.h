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
        m_iBufSize(1000) //necessary size for moving average filt
    {
        buf = new RingBuffer(m_iBufSize);
    }

    ~FilterComponent() override
    {
        m_fSampleRate = 0;
        m_iNumChannels = 0;
        m_iDelayInSamples = 0;

        delete buf;
        m_iBufSize = 0;
    }

    //=========================================================================
    /*
        ***NOTE*** Doesnt seem like this is working???

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
    */

    void processMovingAvgFilt(float* ppfInputBuffer, float* ppfOutputBuffer, int iBufferSize, float fcutoff, float fGain)
    {
        int windowSize = ceil((0.443 / fcutoff) * m_fSampleRate); //CUTOFF RANGE MUST BE 22 Hz - 20 kHz
        for (int i = 0; i < iBufferSize; i++)
        {
            // grab new input sample
            float val = ppfInputBuffer[i];
            buf->pushSample(val);
            float movingSum = 0; //initialize sum
            // go backwards in ringbuffer and sum the previous windowSize elements
            for (int j = 0; j < windowSize; j++)
            {
                movingSum = movingSum + buf->getDelayed(j);
            }

            ppfOutputBuffer[i] = (1.0 / windowSize) * fGain * movingSum;
        }
    }
    
    void processCombFilter(float* ppfInputBuffer, float* ppfOutputBuffer, int iBufferSize, float fDelayTime)
    {
        /* Comb filter implememtation goes here */
    }

private:
    //=========================================================================
    float m_fSampleRate; //from APC
    int m_iNumChannels; //from APC

    int m_iDelayInSamples; //for Comb Filter...from user GUI

    RingBuffer* buf;
    int m_iBufSize;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
