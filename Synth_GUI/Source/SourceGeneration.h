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
        m_iArrayLength = 0;
        m_iSampleRate = 0;
        m_iKarpWriteIdx = 0;
        delete[] m_pfInputArray;
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
    int m_iArrayLength;
    int m_iSampleRate;
    float* m_pfInputArray;
    int m_iKarpWriteIdx;
    
};

class Additive
{
public:
    Additive() : m_dClock(0)
    {}

    ~Additive() {}

    void GetSquareSamp(double& waveSamp, double fs, double amp, double frq, int numHarm)
    {
        waveSamp = 0;

        for (int i = 1; i <= numHarm; i++)
        {
            double temp = 0;
            temp += 0.25 * amp * sin(2 * pi * m_dClock * frq * (2 * i - 1));
            temp /= (2 * i - 1);
            waveSamp += temp;
        }

        waveSamp *= 4 / pi;
        m_dClock += 1/fs;
    }

    void GetTriSamp(double& waveSamp, double fs, double amp, double frq, int numHarm)
    {
        waveSamp = 0;

        for (int i = 1; i <= numHarm; i++)
        {
            double temp = 0;
            temp += amp * sin(2 * pi * m_dClock * frq * (2 * i + 1));
            temp *= pow(-1, i) * pow((2 * i + 1), -2);
            waveSamp += temp;
        }

        waveSamp *= 8 / pow(pi, 2);
        m_dClock += 1 / fs;
    }

private:
    const double pi = 3.141592;
    double m_dClock;
};

