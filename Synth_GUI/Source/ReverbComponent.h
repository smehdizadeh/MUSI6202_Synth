/*
  ==============================================================================

    ReverbComponent.h
    Created: 11 Apr 2021 8:55:26pm
    Author:  sophi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <complex>
#include "impulseAssets.h"

//==============================================================================
/*
*/
class ReverbComponent  : public juce::MPESynthesiser
{
public:
    //==========================================================================
    ReverbComponent(float fSampleRate, int iBlockSize) :
        //forwardFFT(m_fftOrder),
        //fftRev(juce::dsp::Convolution::NonUniform::headSizeInSamples(256)),
        m_fSampleRate(fSampleRate),
        m_iBlockSize(iBlockSize)
        //m_iHopSize(iBlockSize*16),
        //m_bIsReady(false),
        //m_iCounter(0)
    {
        //juce::dsp::Convolution::NonUniform r;
        //r.headSizeInSamples = iBlockSize;
        //fftRev = new juce::dsp::Convolution(r);

        /*
        m_pfIncomingBlocks = new juce::dsp::Complex<float>[m_fftSize];
        //m_pfIncomingBlocks = new float[m_fftSize];
        //set buffer content to zero
        for (int i = 0; i < m_fftSize; i++)
        {
            m_pfIncomingBlocks[i] = 0;
        }
        
        //m_pfIncomingBlocks = juce::dsp::AudioBlock()

        m_pfFFTData = new juce::dsp::Complex<float>[2 * m_fftSize];
        m_pfFFTImpulse = new juce::dsp::Complex<float>[2 * m_fftSize];
        //set buffer content to zero
        for (int i = 0; i < 2*m_fftSize; i++)
        {
            m_pfFFTData[i] = 0;
            m_pfFFTImpulse[i] = 0;
        }

        m_pfOutput = new juce::dsp::Complex<float>[m_fftSize];
        //m_pfOutput = new float[m_fftSize];
        //set buffer content to zero
        for (int i = 0; i < m_fftSize; i++)
        {
            m_pfOutput[i] = 0;
        }

        m_ppfImpulseRespArray = new float *[1];
        m_ppfImpulseRespArray[0] = new float[m_fftSize];
        m_pfCImpulse = new juce::dsp::Complex<float>[m_fftSize];
        //set buffer content to zero
        for (int i = 0; i < m_fftSize; i++)
        {
            m_ppfImpulseRespArray[0][i] = 0;
            m_pfCImpulse[i] = 0;
        }

        //load the impulse response from binary data into buffer
        auto* impulseStream = new juce::MemoryInputStream(impulseAssets::impulseresponse_wav, (size_t)impulseAssets::impulseresponse_wavSize, false);
        juce::WavAudioFormat format;
        juce::AudioFormatReader* impulseReader;
        impulseReader = format.createReaderFor(impulseStream, true);
        if (impulseReader)
        {
            //store impulse data in an array
            impulseReader->read(m_ppfImpulseRespArray, 1, 0, m_fftSize); //trim impulse to fft Size

            for (int i = 0; i < m_fftSize; i++) //copy to the complex array....
            {
                m_pfCImpulse[i] = m_ppfImpulseRespArray[0][i];
            }

            //transform the impulse to frequency domain and store in array
            forwardFFT.perform(m_pfCImpulse, m_pfFFTImpulse, false);
        }
        */

        //m_ppfIncomingData = new float* [1];
        //m_ppfIncomingData[0] = new float[m_iBlockSize];
        m_ppfOutputData = new float[m_iBlockSize];
        for (int i = 0; i < m_iBlockSize; i++)
        {
           // m_ppfIncomingData[0][i] = 0;
            m_ppfOutputData[i] = 0;
        }
        
        //fftRev->loadImpulseResponse(impulseAssets::impulseresponse_wav, (size_t)impulseAssets::impulseresponse_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::no, (size_t)impulseAssets::impulseresponse_wavSize);
        
        auto& conv = fxChain.get<convolutionIndex>();
        conv.loadImpulseResponse(impulseAssets::impulseresponse_wav, (size_t)impulseAssets::impulseresponse_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::no, (size_t)impulseAssets::impulseresponse_wavSize);
        
        //spec.sampleRate = m_fSampleRate;
        //spec.numChannels = 1;
        //spec.maximumBlockSize = impulseAssets::impulseresponse_wavSize;

        //fftRev->prepare(spec); //prepare the convolution... must call before process
        //fxChain.prepare(spec);
        
        //m_pfIncomingBlocks = new juce::dsp::AudioBlock<const float>(m_ppfIncomingData, (size_t)1, (size_t)m_iBlockSize);
        m_pfOutput = new juce::dsp::AudioBlock<float>(&m_ppfOutputData, (size_t)1, (size_t)m_iBlockSize);
        
        
        //contxt = new juce::dsp::ProcessContextNonReplacing<float>;
        
    }

    ~ReverbComponent() override
    {
        //fftRev->reset();
        //fftRev->~Convolution();
        fxChain.reset();

        m_fSampleRate = 0;
        //delete m_pfIncomingBlocks;
        //m_pfIncomingBlocks = 0;
        delete m_pfOutput;
        m_pfOutput = 0;
        /*
        m_iBlockSize = 0;
        m_iHopSize = 0;
        m_bIsReady = false;
        m_iCounter = 0;

        delete[] m_pfIncomingBlocks;
        m_pfIncomingBlocks = 0;

        delete[] m_pfFFTData;
        m_pfFFTData = 0;

        delete[] m_pfOutput;
        m_pfOutput = 0;

        delete[] m_ppfImpulseRespArray[0];
        delete[] m_ppfImpulseRespArray;
        m_ppfImpulseRespArray = 0;

        delete[] m_pfCImpulse;
        m_pfCImpulse = 0;
        */
    }

    //==========================================================================
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept
    {
        fxChain.prepare(spec);
    }

    void renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {
        auto block = juce::dsp::AudioBlock<float>(outputAudio).getSubBlock((size_t)startSample, (size_t)numSamples);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        fxChain.process(context);
    }

    void processConvReverb(float* pfInputBuffer, float* pfOutputBuffer, int iBufferSize, bool isOn)
    {
        auto block = juce::dsp::AudioBlock<float>(&pfOutputBuffer, (size_t)1, (size_t)iBufferSize);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        //contxt = new juce::dsp::ProcessContextNonReplacing<float>(juce::dsp::AudioBlock<const float>(&pfInputBuffer, (size_t)1, (size_t)iBufferSize), m_pfOutput[0]);

        //fftRev->process(context);
        fxChain.process(context);
        
        /*
        if (m_bIsReady) //ready means you have acquired 16 blocks of audio to fill 8160/8190 of IncomingBlocks array
        {
            
            //write incoming block into array at hopSize (end position)
            for (int i = 0; i < iBufferSize; i++)
            {
                m_pfIncomingBlocks[m_iHopSize + i] = pfInputBuffer[i];
            }

            if (isOn) //user turns reverb ON
            {
                //transform to frequency domain
                forwardFFT.perform(m_pfIncomingBlocks, m_pfFFTData, false);

                //multiply with the impulse response
                for (int i = 0; i < 2 * m_fftSize; i++)
                {
                    m_pfFFTData[i] *= m_pfFFTImpulse[i];
                }

                //transform back to time domain
                forwardFFT.perform(m_pfFFTData, m_pfOutput, true);

                //copy to output audio buffer
                for (int i = 0; i < iBufferSize; i++)
                {
                    pfOutputBuffer[i] = abs(m_pfOutput[i]); //complex to float
                }

            }
            else if (!isOn) //if user turns OFF
            {
                //straight passthrough to output
                for (int i = 0; i < iBufferSize; i++)
                {
                    pfOutputBuffer[i] = pfInputBuffer[i];
                }
            }

            //shift over to prepare for next block
            for (int i = 0; i < m_iHopSize; i++)
            {
                m_pfIncomingBlocks[i] = m_pfIncomingBlocks[m_iBlockSize + i];
            }
        }

        else if (!m_bIsReady) //if the array is not yet full
        {
            //write the incoming block into the array according to the counter
            for (int i = 0; i < iBufferSize; i++)
            {
                m_pfIncomingBlocks[m_iCounter*m_iBlockSize + i] = pfInputBuffer[i];
            }

            //increment the counter
            m_iCounter = m_iCounter + 1;

            //check if ready
            if (m_iCounter == 16)
            {
                m_bIsReady = true;
            }
        }

        
        */
        //normalize???
        /*
        for (int i = 0; i < 2 * m_fftSize; i++)
        {
            m_pfFFTData[i] /= m_fftSize;
        }
        */
        
    }

    //static constexpr auto m_fftOrder = 13; //represents size of FFT (2^fftOrder)
    //static constexpr auto m_fftSize = 1 << m_fftOrder; //binary 2^fftOrder

private:
    //==========================================================================
    //juce::dsp::FFT forwardFFT; //size of this needs to be next largest power of 2 after (2*block size)
    
    //r.headSizeInSamples = iBlockSize;
    enum
    {
        convolutionIndex
    };

    juce::dsp::ProcessorChain < juce::dsp::Convolution > fxChain;
    //juce::dsp::Convolution* fftRev;
    //juce::dsp::ProcessSpec spec;
    //juce::dsp::ProcessContextNonReplacing<float>* contxt;
    int m_iBlockSize; //size of each new incoming audio block from APC
    //int m_iHopSize; //same as block size for 50% overlap, since we process two blocks at a time
    float m_fSampleRate;
    //bool m_bIsReady; //when running for first time must wait to fill array (17 blocks)
    //int m_iCounter; //detect when array is full and ready to process
    
    //juce::dsp::Complex<float>* m_pfIncomingBlocks; //size of this needs to be next largest power of 2 after (2*block size)
                                                   //last 64 samples will always be zero padded
    //juce::dsp::ProcessContextNonReplacing<float>::AudioBlockType m_pfIncomingBlocks;
    //float** m_ppfImpulseRespArray;    //where impulse response data is stored (loaded from binary)
    //float** m_ppfIncomingData;
    //const juce::dsp::AudioBlock<const float> m_pfIncomingBlocks;
    //juce::dsp::Complex<float>* m_pfCImpulse; //because this has to be a complex float imput for some reason....

    //juce::dsp::Complex<float>* m_pfFFTImpulse;          //where frequency transformed impulse is stored
    //juce::dsp::Complex<float>* m_pfFFTData;        //where FFT transformed data is stored (2* IncomingBlock array)

    //juce::dsp::Complex<float>* m_pfOutput;         //where inverse FFT output is stored
    float* m_ppfOutputData;
    juce::dsp::AudioBlock<float>* m_pfOutput;
    //float* m_pfOutput;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbComponent)
};
