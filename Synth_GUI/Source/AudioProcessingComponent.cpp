/*
  ==============================================================================

    AudioProcessingComponent.cpp
    Created: 31 Mar 2021 11:52:36am
    Author:  sophi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioProcessingComponent.h"
#include <cmath>


//==============================================================================
AudioProcessingComponent::AudioProcessingComponent() :
    Add(0),
    m_pfSoundArray(0),
    m_fFreq(0),
    m_fSampleRate(0),
    m_iNumChannels(2),
    KS(0),
    filt(0),
    key(0),
    m_dWaveSamp(0),
    m_dTime(0),
    m_fSampExpect(0),
    revrb(0),
    m_bPlaying(false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setAudioChannels(0, m_iNumChannels); // no inputs, two outputs
    m_kSource = Source::square;
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();
    audioBuffer.clear();

    delete[] m_pfSoundArray;

    KS->~KarplusStrong();
    KS = 0;
  
    Add->~Additive();
    Add = 0;

    filt->~FilterComponent();
    filt = 0;
  
    revrb->~ReverbComponent();
    revrb = 0;

    env.reset();
}


//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_fSampleRate = sampleRate;
    m_fSampExpect = samplesPerBlockExpected;

    filt = new FilterComponent(m_fSampleRate); //create filter module
    revrb = new ReverbComponent(m_fSampleRate, samplesPerBlockExpected); //create reverb module

    // Code for Karplus Strong Algorithm
    m_fFreq = 440; //KS frequency
    KS = new KarplusStrong(m_fSampleRate); //create KS generator
    KS->CreateOutput();
    m_pfSoundArray = new float[m_fSampleRate]; //KS buffer
    KS->GetKarpArray(m_pfSoundArray);

    Add = new Additive(0);
  
    //Envelope
    env.setSampleRate(sampleRate);
    juce::ADSR::Parameters params;
    params.attack = 0;
    params.decay = 0;
    params.sustain = 1.0;
    params.release = 0.5;
    env.setParameters(params);
  
    audioBuffer.setSize(1, samplesPerBlockExpected); //mono working buffer
    audioBuffer.clear();
}



void AudioProcessingComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // ***** EVERYTHING DONE IN MONO AND THEN COPIED TO ADDITIONAL CHANNELS ******

    // Filter controls
    if (key.isKeyCurrentlyDown(juce::KeyPress::upKey))
    {
        filt->AdjustCutoffFreq(juce::KeyPress::upKey);
    }

    if (key.isKeyCurrentlyDown(juce::KeyPress::downKey))
    {
        filt->AdjustCutoffFreq(juce::KeyPress::downKey);
    }

    auto* p = audioBuffer.getWritePointer(0); //pointer to working buffer


    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        m_fFreq = GetInput();

        //apply ADSR accordingly
        if (m_bPlaying)
        {

            //AdditiveBlock
            if (m_kSource == Source::square)
            {
                Add->GetSample(m_dWaveSamp, m_dTime, m_fSampleRate, 1, GetInput(), 10); // 3rd and four
                p[sample] = m_dWaveSamp;
            }

            else 
            {
                //KarplusBlock
                p[sample] = m_pfSoundArray[KS->GetKarpWriteIdx()];
            }
            env.noteOn();
        }
        else
        {
            env.noteOff();
        }
        KS->SetKarpWriteIdx((int)(m_fSampleRate/m_fFreq));
    }
    env.applyEnvelopeToBuffer(audioBuffer, 0, bufferToFill.numSamples);

    // CUTOFF RANGE IS 22 Hz - 20 kHz, GAIN RANGE IS 0.0 - 1.0
    filt->processMovingAvgFilt(p, p, bufferToFill.numSamples, filt->GetCutoffFreq(), 0.9); //LP Filter

    // CONVOLUTIONAL REVERB TESTING.. PRESS TAB TO TOGGLE REVERB ON/OFF
    if (key.isKeyCurrentlyDown(juce::KeyPress::tabKey))
    {
        revrb->renderNextSubBlock(audioBuffer, 0, bufferToFill.numSamples);
    }

    // send to the Juce output buffer (ALL CHANNELS)
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, p, bufferToFill.numSamples);
    }
}

void AudioProcessingComponent::releaseResources()
{
    
}

double AudioProcessingComponent::SetFrq(double m)
{
    m_bPlaying = true;
    return pow(2, (m - 69) / 12) * 440;
}

double AudioProcessingComponent::GetInput()
{
    if (key.isKeyCurrentlyDown((int)Note::c))
    {
        return SetFrq(48);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::cs)))
    {
        return SetFrq(49);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::d)))
    {
        return SetFrq(50);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::ds)))
    {
        return SetFrq(51);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::e)))
    {
        return SetFrq(52);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::f)))
    {
        return SetFrq(53);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::fs)))
    {
        return SetFrq(54);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::g)))
    {
        return SetFrq(55);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::gs)))
    {
        return SetFrq(56);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::a)))
    {
        return SetFrq(57);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::as)))
    {
        return SetFrq(58);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::b)))
    {
        return SetFrq(59);
    }
    else if ((key.isKeyCurrentlyDown((int)Note::cOct)))
    {
        return SetFrq(60);
    }
    else
    {
        m_bPlaying = false;
        return 0;
    }
}

void AudioProcessingComponent::NextSource()
{
    if (m_kSource == Source::square)
    {
        m_kSource = Source::karplus;
    }
    else if (m_kSource == Source::karplus)
    {
        m_kSource = Source::square;
    }
}

//enum Source
//{
//    additive,
//    karplus
//};
//
//void ChangeSource(Source source)
//{
//    m_kSource = source;
//}