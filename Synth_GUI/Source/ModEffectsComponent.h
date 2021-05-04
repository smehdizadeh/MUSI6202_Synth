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
#include <math.h>

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
        m_iBufSize(1000),
        m_fPhiVibrato(0),
        m_fPhiFlange(0),
        m_fPhiChorus(0)

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
        m_fPhiVibrato = 0;
        m_fPhiFlange = 0;
        m_fPhiChorus = 0;
        //delete lfo;
        delete buf;
        m_iBufSize = 0;
    }

    //========================================================================
    void processFlanger(float* ppfInputBuffer, float* ppfOutputBuffer, int iBufferSize, float flangeFrequency)
    {
        float deltaPhi = flangeFrequency / (m_fSampleRate);
        // FLANGER EFFECT W/ FEEDBACK GOES HERE
        int A = static_cast < int>(0.002 * m_fSampleRate);
        int M = static_cast<int>(0.002 * m_fSampleRate);
        float BL = 0.7;
        float FF = 0.7;

        for (int sample = 0; sample < iBufferSize; ++sample) 
        {
            float curSamp = ppfInputBuffer[sample];
            buf->pushSample(curSamp);

            // delay the delayedSamp by a delay given by the lfo.
            // implement the A, M, BL, and FF later!!!
            int delay = static_cast<int>(sin(2 * M_PI * m_fPhiFlange) * A) + M;
            float delayed = buf->getDelayed(delay);
            float output = (curSamp * BL) + (delayed * FF);

            m_fPhiFlange = m_fPhiFlange + deltaPhi;
            while (m_fPhiFlange >= 1) {
                m_fPhiFlange = m_fPhiFlange - 1;
            }

            ppfOutputBuffer[sample] = output;
        }
    }

    void processVibrato(float* ppfInputBuffer, float* ppfOutputBuffer, int iBufferSize, float vibeFrequency)
    {
        float deltaPhi = vibeFrequency/(m_fSampleRate);
        
        // VIBRATO EFFECT GOES HERE
        for (int sample = 0; sample < iBufferSize; ++sample)
        {
            float curSamp = ppfInputBuffer[sample];
            // delay the curSamp by an lfo.
            buf->pushSample(curSamp);

            int delay = static_cast<int>((sin(2 * M_PI * m_fPhiVibrato) + 1.00) * m_iBufSize/2.0);
            float output = buf->getDelayed(delay) * 0.5; //not sure why the multiplying by 0.5

            m_fPhiVibrato = m_fPhiVibrato + deltaPhi;
            while (m_fPhiVibrato >= 1){
                m_fPhiVibrato = m_fPhiVibrato - 1;
            }

            ppfOutputBuffer[sample] = output;
        }
    }
    void processChorus(float* ppfInputBuffer, float* ppfOutputBuffer, int iBufferSize, float chorusFrequency)
    {
        float deltaPhi = chorusFrequency / (m_fSampleRate);
        
        int A = static_cast <int>(0.002 * m_fSampleRate);
        int M = static_cast<int>(0.002 * m_fSampleRate);
        float BL = 1.0;
        float FF = 0.7;

        // CHORUS EFFECT GOES HERE
        for (int sample = 0; sample < iBufferSize; ++sample)
        {
            float curSamp = ppfInputBuffer[sample];
            // delay the curSamp by an lfo.
            buf->pushSample(curSamp);

            int delay = static_cast<int>(sin(2 * M_PI * m_fPhiChorus)*A) + M;
            float delayed = buf->getDelayed(delay);
            float output = (curSamp * BL) + (delayed * FF); 

            // increment phi by delta phi for each sample in the loop
            m_fPhiChorus = m_fPhiChorus + deltaPhi;       
            while (m_fPhiChorus >= 1) {
                m_fPhiChorus = m_fPhiChorus - 1;
            }

            ppfOutputBuffer[sample] = output;
        }
    }

private:
    //=======================================================================
    float m_fSampleRate; //from APC
    float m_fFreq;
//    LFOComponent* lfo;
    float m_fPhiVibrato;
    float m_fPhiFlange;
    float m_fPhiChorus;

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