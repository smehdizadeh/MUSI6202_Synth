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
#include <windows.h> //For Debug macro
#include <debugapi.h> //For Debug macro

// Use DBOUT(args) to print to the VS console
#define DBOUT(s)            \
{                             \
std::ostringstream os_;    \
os_ << s;                   \
OutputDebugString(os_.str().c_str()); \
OutputDebugString("\n"); \
}


//==============================================================================
AudioProcessingComponent::AudioProcessingComponent() :
    Add(0),
    m_pfSoundArray(0),
    m_dFreq(0),
    m_fSampleRate(0),
    m_fOutputSampRate(0),
    m_iNumChannels(2),
    KS(0),
    filt(0),
    key(0),
    m_dWaveSamp(0),
    m_dTime(0),
    m_fSampExpect(0),
    revrb(0),
    m_bPlaying(false),
    m_iNumKeysDown(0)
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
    antiAlias.reset();
}


//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_fSampleRate = sampleRate;

    filt = new FilterComponent(m_fSampleRate); //create filter module
    revrb = new ReverbComponent(m_fSampleRate, samplesPerBlockExpected); //create reverb module

    // Code for Karplus Strong Algorithm 
    KS = new KarplusStrong(m_fSampleRate); //create KS generator
    KS->CreateOutput();
    m_pfSoundArray = new float[m_fSampleRate]; //KS buffer
    KS->GetKarpArray(m_pfSoundArray);

    Add = new Additive();
  
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
       
        //AdditiveBlock
        if (m_kSource == Source::square)
        {
            Add->GetSquareSamp(m_dWaveSamp, m_dTime, m_fSampleRate, 1, m_dFreq, 10); // 3rd and four
            p[sample] = m_dWaveSamp;
        }

        else if (m_kSource == Source::karplus)
        {
            p[sample] = m_pfSoundArray[KS->GetKarpWriteIdx()];
        }

        else if (m_kSource == Source::triangle)
        {
            Add->GetTriSamp(m_dWaveSamp, m_dTime, m_fSampleRate, 1, m_dFreq, 10); // 3rd and four
            p[sample] = m_dWaveSamp;
        }

        //apply ADSR accordingly
        if (m_bPlaying)
        {
            env.noteOn();
        }
        else
        {
            env.noteOff();
        }

        KS->SetKarpWriteIdx((int)(m_fSampleRate/m_dFreq));
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
        //perform downsampling here
        changeSampleRate(p, bufferToFill.numSamples);

        //copy to output channels
        bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, p, bufferToFill.numSamples);
    }
}

void AudioProcessingComponent::releaseResources()
{
    
}

void AudioProcessingComponent::ToggleSynth()
{
    if (key.isKeyCurrentlyDown((int)Note::c))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::cs)))
    {

    }
    else if ((key.isKeyCurrentlyDown((int)Note::d)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::ds)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::e)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::f)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::fs)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::g)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::gs)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::a)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::as)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::b)))
    {
        
    }
    else if ((key.isKeyCurrentlyDown((int)Note::cOct)))
    {
        
    }
    else
    {
        m_bPlaying = false;
    }
}

void AudioProcessingComponent::SetFrq(double m)
{
    m_bPlaying = true;
    m_iNumKeysDown += 1;
    m_dFreq = pow(2, (m - 69) / 12) * 440;
}


void AudioProcessingComponent::GetKey(int press)
{
    switch (press)
    {
    case (int)Note::c:
        SetFrq(48);
        break;
    case (int)Note::cs:
        SetFrq(49);
        break;
    case (int)Note::d:
        SetFrq(50);
        break;
    case (int)Note::ds:
        SetFrq(51);
        break;
    case (int)Note::e:
        SetFrq(52);
        break;
    case (int)Note::f:
        SetFrq(53);
        break;
    case (int)Note::fs:
        SetFrq(54);
        break;
    case (int)Note::g:
        SetFrq(55);
        break;
    case (int)Note::gs:
        SetFrq(56);
        break;
    case (int)Note::a:
        SetFrq(57);
        break;
    case (int)Note::as:
        SetFrq(58);
        break;
    case (int)Note::b:
        SetFrq(59);
        break;
    case (int)Note::cOct:
        SetFrq(60);
        break;
    default:
        m_bPlaying = false;
        break;
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
        m_kSource = Source::triangle;
    }
    else if (m_kSource == Source::triangle)
    {
        m_kSource = Source::square;
    }
}

void AudioProcessingComponent::setSampleRate(float newSampRate)
{
    m_fOutputSampRate = newSampRate;
}

void AudioProcessingComponent::changeSampleRate(float* pfAudio, int numSamples)
{
    //first check for default/no change case  (48k)
    if (m_fOutputSampRate == 48000.0) { return; }

    else
    {
        //apply anti-aliasing filter
        antiAlias.reset();
        antiAlias.setCoefficients(juce::IIRCoefficients::makeLowPass(m_fSampleRate, m_fOutputSampRate / 2.0));
        antiAlias.processSamples(pfAudio, numSamples);

        if (m_fOutputSampRate == 16000.0) //then check for the integer factor case (16k)
        {
            for (int i = 0; i < numSamples; i += 3) //take every third sample
            {

            }
        }
        else //rational factor
        {

        }
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
