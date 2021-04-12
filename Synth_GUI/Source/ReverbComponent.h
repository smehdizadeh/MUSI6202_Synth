/*
  ==============================================================================

    ReverbComponent.h
    Created: 11 Apr 2021 8:55:26pm
    Author:  sophi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RingBuffer.h"

//==============================================================================
/*
*/
class ReverbComponent  : public juce::Component
{
public:
    //==========================================================================
    ReverbComponent(float fSampleRate, int iBlockSize) :
        forwardFFT(m_fftOrder),
        m_fSampleRate(fSampleRate),
        m_iBlockSize(iBlockSize),
        m_iHopSize(iBlockSize)
    {
        m_pfIncomingBlocks = new juce::dsp::Complex<float>[m_fftSize];
        //set buffer content to zero
        for (int i = 0; i < m_fftSize; i++)
        {
            m_pfIncomingBlocks[i] = 0;
        }

        m_pfFFTData = new juce::dsp::Complex<float>[2 * m_fftSize];
        //set buffer content to zero
        for (int i = 0; i < 2*m_fftSize; i++)
        {
            m_pfFFTData[i] = 0;
        }

        m_pfOutput = new juce::dsp::Complex<float>[m_fftSize];
        //set buffer content to zero
        for (int i = 0; i < m_fftSize; i++)
        {
            m_pfOutput[i] = 0;
        }
    }

    ~ReverbComponent() override
    {
        m_fSampleRate = 0;
        m_iBlockSize = 0;
        m_iHopSize = 0;

        delete[] m_pfIncomingBlocks;
        m_pfIncomingBlocks = 0;

        delete[] m_pfFFTData;
        m_pfFFTData = 0;

        delete[] m_pfOutput;
        m_pfOutput = 0;
    }

    //==========================================================================
    void processConvReverb(float* pfInputBuffer, float* pfOutputBuffer, int iBufferSize)
    {
        //write incoming block into array at hopSize
        for (int i = 0; i < iBufferSize; i++)
        {
            m_pfIncomingBlocks[m_iHopSize + i] = pfInputBuffer[i];
        }

        //transform to frequency domain
        forwardFFT.perform(m_pfIncomingBlocks, m_pfFFTData, false);

        //....

        //transform back to time domain
        forwardFFT.perform(m_pfFFTData, m_pfOutput, true);

        //copy to output audio buffer
        for (int i = 0; i < iBufferSize; i++)
        {
            pfOutputBuffer[i] = static_cast<float>(m_pfOutput[i]);
        }

        //shift most recent block over
        for (int i = 0; i < iBufferSize; i++)
        {
            m_pfIncomingBlocks[i] = m_pfIncomingBlocks[m_iHopSize + i];
        }
    }

    static constexpr auto m_fftOrder = 10; //represents size of FFT (2^fftOrder)
    static constexpr auto m_fftSize = 1 << m_fftOrder; //binary 2^fftOrder

private:
    //==========================================================================
    juce::dsp::FFT forwardFFT; //size of this needs to be next largest power of 2 after (2*block size)
    int m_iBlockSize; //size of each new incoming audio block from APC
    int m_iHopSize; //same as block size for 50% overlap, since we process two blocks at a time
    float m_fSampleRate;
    
    juce::dsp::Complex<float>* m_pfIncomingBlocks; //size of this needs to be next largest power of 2 after (2*block size)
                                                   //last 64 samples will always be zero padded
    juce::dsp::Complex<float>* m_pfFFTData;        //where FFT transformed data is stored (2* IncomingBlock array)
    juce::dsp::Complex<float>* m_pfOutput;         //where inverse FFT output is stored

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbComponent)
};
