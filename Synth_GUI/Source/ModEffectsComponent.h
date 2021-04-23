/*
  ==============================================================================

    ModEffectsComponent.h
    Created: 6 Apr 2021 10:23:51am
    Author:  sophi

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES

#include <JuceHeader.h>
#include <cmath>
#include "RingBuffer.h"
const float pi = 3.14159265358979323846;

//==============================================================================
/*
*/
class ModEffectsComponent  : public juce::Component
{
public:
    //=========================================================================
    ModEffectsComponent(float fSampleRate) :
        m_fFreq(0),
        m_fSampleRate(fSampleRate),
        m_iBufSize(1000)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        //lfo = new LFOComponent(m_fFreq,m_fSampleRate);
        buf = new RingBuffer(m_iBufSize);
    }

    ~ModEffectsComponent() override
    {
        m_fFreq = 0;
        m_fSampleRate = 0;
        //delete lfo;
        delete buf;
        m_iBufSize = 0;
    }

    //========================================================================
    void processFlanger(float* ppfInputBuffer, float* ppfOutputBuffer, int iBufferSize, float flangeFrequency)
    {
        // FLANGER EFFECT W/ FEEDBACK GOES HERE
        for (int sample = 0; sample < iBufferSize; ++sample) 
        {
            float curSamp = ppfInputBuffer[sample];
            float delayedSamp = ppfInputBuffer[sample];
            // delay the delayedSamp by a delay given by the lfo wavetable somehow - I DONT GET HOW IT WORKS
            float delay = 0.0;
            delayedSamp = delayedSamp + delay;

            // feedback:
            ppfOutputBuffer[sample] = (curSamp * 0.7) + (delayedSamp * 0.7);
        }
    }

    void processVibrato(float* ppfInputBuffer, float* ppfOutputBuffer, int iBufferSize, float vibeFrequency)
    {
        float deltaPhi = vibeFrequency / m_fSampleRate;
        float phi = 0;
        // VIBRATO EFFECT GOES HERE
        for (int sample = 0; sample < iBufferSize; ++sample)
        {
            float curSamp = ppfInputBuffer[sample];
            // delay the curSamp by an lfo.
            buf->pushSample(curSamp);

 
            float delay = static_cast<float>((sin(2 * pi * phi) + 1.001) * iBufferSize);
            float output = buf->getDelayed(delay) * 0.5; //not sure why the multiplying by 0.5

            phi = phi + deltaPhi;
            while (phi >= 1){
                phi = phi - 1;
            }
            

            ppfOutputBuffer[sample] = output;
        }

    }

private:
    //=======================================================================
    float m_fSampleRate; //from APC
    float m_fFreq;
//    LFOComponent* lfo;

    RingBuffer* buf;
    int m_iBufSize;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModEffectsComponent)
};


//===========================================================================
class LFOComponent : public juce::Component
{
public:
    //=======================================================================
    LFOComponent(float fFreq, float fSampRate) :
        m_fFreq(fFreq),
        m_fSampleRate(fSampRate)
    {
        if (fFreq == 0)
            m_iTableSize = 1;
        else
            m_iTableSize = int(floor(m_fSampleRate / fFreq));

        table = new RingBuffer(m_iTableSize);

        calculateWavetable();
    }

    ~LFOComponent() override
    {
        m_fSampleRate = 0;
        m_iTableSize = 0;

        delete table;
        table = 0;
    }

private:
    //======================================================================
    void calculateWavetable()
    {
        for (int i = 0; i < m_iTableSize; i++)
        {
            table->pushSample(static_cast<float>(sin(2 * pi * i * m_fFreq / m_fSampleRate)));
        }
    }

    //======================================================================
    float m_fSampleRate;
    float m_fFreq; // from user GUI
    int m_iTableSize;
    RingBuffer* table; // lfo wavetable storage

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};