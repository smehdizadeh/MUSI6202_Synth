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

//==============================================================================
/*
*/
class ModEffectsComponent  : public juce::Component
{
public:
    //=========================================================================
    ModEffectsComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~ModEffectsComponent() override
    {
    }

    //========================================================================
    void processFlanger()
    {
        // FLANGER EFFECT W/ FEEDBACK GOES HERE
    }

    void processVibrato()
    {
        // VIBRATO EFFECT GOES HERE
    }

private:
    //=======================================================================
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
            table->pushSample(static_cast<float>(sin(2 * M_PI * i * m_fFreq / m_fSampleRate)));
        }
    }

    //======================================================================
    float m_fSampleRate;
    float m_fFreq; // from user GUI
    int m_iTableSize;
    RingBuffer* table; // lfo wavetable storage

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};