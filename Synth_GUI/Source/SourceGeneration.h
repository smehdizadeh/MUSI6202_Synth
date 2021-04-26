/*
  ==============================================================================

    SourceGeneration.h
    Created: 3 Apr 2021 6:50:34pm
    Author:  ethri

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "RingBuffer.h"
#include <math.h>

class KarplusStrong
{
public:
    KarplusStrong(double fs)
    {
        m_iArrayLength = fs; // Determines Frequency 
        m_iSampleRate = (int)fs;
        m_iKarpWriteIdx = 0;
        m_pfInputArray = new float[m_iArrayLength];
    }

    ~KarplusStrong()
    {
        //attenuation = 0;
        m_iArrayLength = 0;
        m_iSampleRate = 0;
        m_iKarpWriteIdx = 0;
        delete[] m_pfInputArray;
        //delete[] karpArray;
    }

    int GetDelayLength()
    {
        return m_iArrayLength;
    }

    void GetKarpArray(float* arr)
    {
        for (int i = 0; i < m_iArrayLength; i++)
        {
            arr[i] = m_pfInputArray[i];
        }
    }

    int GetKarpWriteIdx()
    {
        return m_iKarpWriteIdx;
    }

    void SetKarpWriteIdx(int cap)
    {
        m_iKarpWriteIdx = (m_iKarpWriteIdx + 1) % cap;

        if (m_iKarpWriteIdx < 0)
        {
            m_iKarpWriteIdx = 0;
        }

        if (m_iKarpWriteIdx >= cap)
        {
            m_iKarpWriteIdx = 0;
        }
    }

    void CreateOutput()
    {
        for (int i = 0; i < m_iArrayLength; i++)
        {
            m_pfInputArray[i] = random.nextFloat() * 0.25f - 0.125f;
        }
    }

private:
    juce::Random random;
    //float attenuation;
    int m_iArrayLength;
    int m_iSampleRate;
    float* m_pfInputArray;
    //float* karpArray;
    int m_iKarpWriteIdx;
    
};

class Additive
{
public:
    Additive(float frq) :
        m_fFrq(frq),
        m_fLevel(0)
    {
        //addBuff = new RingBuffer((int)fs);
    }

    ~Additive()
    {

    }

    void SetFrq(float frq)
    {
        m_fFrq = frq;
    }

    void SetLevel(float level)
    {
        m_fLevel = level;

        if (m_fLevel > 1)
        {
            m_fLevel = 1;
        }
    }

    /*enum Shape
    {
        kSine,
        kSquare,
        kNumShapes
    };*/

    void GetSample(double& waveSamp, double& time, double fs, double amp, double frq, double numHarm)
    {
        waveSamp = 0;
        if (time >= 1)
        {
            time = 0;
        }

        for (int i = 1; i <= numHarm; i++)
        {
            waveSamp += amp * sin(2 * pi * time * frq * (__int64)(2 * i - 1));
            waveSamp /= (__int64)(2 * i - 1);
        }

        waveSamp *= 4 / pi;
        time += 1 / fs;
    }

private:
    const double pi = 3.141592;
    float m_fFrq;
    float m_fLevel;
    //Shape m_WaveShape;

    //RingBuffer* addBuff;
};

//class Granular :: Unused
//{
//public:
//    Granular(int grainSize, int hopSize, float frq, int tableSize) :
//        m_iGrainSize(grainSize),
//        m_iHopSize(hopSize),
//        m_dGrainPitch(frq),
//        m_iTableSize(tableSize)
//    {
//        wavetable = new RingBuffer(m_iTableSize);
//    }
//
//    ~Granular()
//    {
//        m_iGrainSize = 0;
//        m_iHopSize = 0;
//        m_dGrainPitch = 0;
//
//        delete wavetable;
//        m_iTableSize = 0;
//    }
//
//    void SetHopSize(int hop)
//    {
//        m_iHopSize = hop;
//    }
//
//    void SetPitch()
//    {
//
//    }
//
//private:
//
//    int m_iGrainSize;
//    int m_iHopSize;
//    float m_dGrainPitch;
//
//    RingBuffer* wavetable;
//    int m_iTableSize;
//};