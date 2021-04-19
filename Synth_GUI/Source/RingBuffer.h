/*
  ==============================================================================
    RingBuffer.h
    Created: 3 Apr 2021 11:15:45pm
    Author:  sophi
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RingBuffer : public juce::Component
{
public:
    //==========================================================================
    RingBuffer(int iMaxDelay) :
        m_iRingBuffSize(iMaxDelay),
        m_iWriteIdx(0)
    {
        m_pfBuff = new float[m_iRingBuffSize]; //allocate memory
        //set buffer content to zero
        for (int i = 0; i < m_iRingBuffSize; i++)
        {
            m_pfBuff[i] = 0;
        }
    }

    ~RingBuffer() override
    {
        m_iRingBuffSize = 0;
        m_iWriteIdx = 0;

        delete[] m_pfBuff;
        m_pfBuff = 0;
    }

    //=========================================================================
    void pushSample(float fSample)
    {
        m_pfBuff[m_iWriteIdx] = fSample; //write new value
        m_iWriteIdx = (m_iWriteIdx + 1) % m_iRingBuffSize; //increment write index
    }

    float getDelayed(int d)
    {
        int readIdx = ((m_iWriteIdx + m_iRingBuffSize) - d - 1) % m_iRingBuffSize; //get a read index for delayed sample
        return m_pfBuff[readIdx];
    }

private:
    //=========================================================================
    int m_iRingBuffSize;
    float* m_pfBuff;

    int m_iWriteIdx;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingBuffer)
};
