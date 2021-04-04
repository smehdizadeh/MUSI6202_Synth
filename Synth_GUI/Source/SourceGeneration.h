/*
  ==============================================================================

    SourceGeneration.h
    Created: 3 Apr 2021 6:50:34pm
    Author:  ethri

  ==============================================================================
*/

#pragma once
#include <cmath>
#include <JuceHeader.h>

class KarplusStrong
{
public:
    KarplusStrong(double alpha, int M, double fs)
    {
        delayLength = M;
        attenuation = pow(alpha, (delayLength / 50));
        sampleRate = (int)fs;
        karpWriteIdx = 0;
        inputArray = new float[delayLength];
        karpArray = new float[sampleRate];
    }

    ~KarplusStrong()
    {
        attenuation = 0;
        delayLength = 0;
        karpWriteIdx = 0;
        //delete[] inputArray;
        delete[] karpArray;
    }

    int GetDelayLength()
    {
        return delayLength;
    }

    void GetKarpArray(float* arr)
    {
        for (int i = 0; i < sampleRate; i++)
        {
            arr[i] = karpArray[i];
        }
    }

    int GetKarpWriteIdx()
    {
        return karpWriteIdx;
    }

    void SetKarpWriteIdx()
    {
        karpWriteIdx = (karpWriteIdx + 1) % sampleRate;

        if (karpWriteIdx < 0)
        {
            karpWriteIdx = 0;
        }

        if (karpWriteIdx >= delayLength)
        {
            karpWriteIdx = 0;
        }
    }

    void CreateOutput()
    {
        for (int i = 0; i < delayLength; i++)
        {
            inputArray[i] = random.nextFloat() * 0.25f - 0.125f;
        }

        for (int i = 0; i < sampleRate; i++)
        {
            if (i < delayLength)
            {
                karpArray[i] = inputArray[i];
            }

            else
            {
                karpArray[i] = attenuation * karpArray[i - delayLength];
            }
        }
    }

private:
    juce::Random random;
    float attenuation;
    int delayLength;
    int sampleRate;
    float* inputArray;
    float* karpArray;
    int karpWriteIdx;
};
