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
    m_pfSoundArray(0),
    m_dFreq(0),
    m_fSampleRate(0),
    m_fOutputSampRate(0),
    m_fOutputBitDepth(0),
    m_iNumChannels(2),

    m_dWaveSamp(0),
    m_iNumKeysDown(0),
    m_kSource(Source::square),
    m_dTransposeVal(0),
    m_bPlaying(false),
    m_fNumHarmonics(200),

    m_bReverbOn(false),
    m_bDitherOn(false),
    m_fLpfCutoff(3000),
    m_iCombFilterVal(500),
    m_fFlangerFrq(500),
    m_fChorusFrq(500),
    m_fVibratoFrq(500),

    effects(0),
    filt(0),
    KS(0),
    revrb(0),
    Add(0),
    mod(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setAudioChannels(0, m_iNumChannels); // no inputs, two outputs
}

AudioProcessingComponent::~AudioProcessingComponent()
{
    shutdownAudio();
    audioBuffer.clear();

    delete[] m_pfSoundArray;
    delete[] effects;

    KS->~KarplusStrong();
    KS = 0;
  
    Add->~Additive();
    Add = 0;

    filt->~FilterComponent();
    filt = 0;
  
    revrb->~ReverbComponent();
    revrb = 0;

    mod->~ModEffectsComponent();
    mod = 0;

    env.reset();
    antiAlias.reset();
}


//=============================================================================
void AudioProcessingComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_fSampleRate = sampleRate;
    m_fOutputSampRate = sampleRate; 
    m_fOutputBitDepth = 32.0;

    effects = new Effects[6]{ Effects::none, Effects::none, Effects::none, Effects::none, Effects::none, Effects::none };

    filt = new FilterComponent(m_fSampleRate); //create filter module
    revrb = new ReverbComponent(m_fSampleRate, samplesPerBlockExpected); //create reverb module
    mod = new ModEffectsComponent(m_fSampleRate); // create modulated effects module

    // Code for Karplus Strong Algorithm 
    KS = new KarplusStrong(m_fSampleRate); //create KS generator
    KS->CreateOutput(); //Create KS random samples
    m_pfSoundArray = new float[m_fSampleRate]; //KS buffer
    KS->GetKarpArray(m_pfSoundArray); // Put random samples into buffer

    Add = new Additive();
  
    //Envelope
    env.setSampleRate(sampleRate);
    juce::ADSR::Parameters params;
    params.attack = 0.1;
    params.decay = 0.1;
    params.sustain = 0.9;
    params.release = 0.5;
    env.setParameters(params);
  
    audioBuffer.setSize(1, samplesPerBlockExpected); //mono working buffer
    audioBuffer.clear();

    //audioSetup.bufferSize = samplesPerBlockExpected;
    //audioSetup.sampleRate = sampleRate;
    //manager.setAudioDeviceSetup(audioSetup, false);
}



void AudioProcessingComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // ***** EVERYTHING DONE IN MONO AND THEN COPIED TO ADDITIONAL CHANNELS ******

    auto* p = audioBuffer.getWritePointer(0); //pointer to working buffer

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
       
        if (m_kSource == Source::square)
        {
            Add->GetSquareSamp(m_dWaveSamp, m_fSampleRate, 1, m_dFreq, m_fNumHarmonics); 
            p[sample] = m_dWaveSamp;
        }

        else if (m_kSource == Source::sine)
        {
            Add->GetSquareSamp(m_dWaveSamp, m_fSampleRate, 1, m_dFreq, 1);
            p[sample] = m_dWaveSamp;
        }

        else if (m_kSource == Source::karplus)
        {
            p[sample] = m_pfSoundArray[KS->GetKarpWriteIdx()];
        }

        else if (m_kSource == Source::triangle)
        {
            Add->GetTriSamp(m_dWaveSamp, m_fSampleRate, 1, m_dFreq, m_fNumHarmonics); 
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

    ModuleManager(effects, audioBuffer, 0, bufferToFill.numSamples, p, m_fLpfCutoff, 0.9); //handle all effects here

    // send to the Juce output buffer (ALL CHANNELS)
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        //perform downsampling here
        changeSampleRate(p, bufferToFill.numSamples);

        //perform down quantizing here
        changeBitDepth(p, bufferToFill.numSamples);

        //copy to output channels
        bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, p, bufferToFill.numSamples);
    }
}

void AudioProcessingComponent::releaseResources()
{
    
}

void AudioProcessingComponent::setFrq(double m)
{
    m += m_dTransposeVal;
    m_bPlaying = true;
    m_iNumKeysDown += 1;
    m_dFreq = pow(2, (m - 69) / 12) * 440;
}

void AudioProcessingComponent::setTranspositionVal(double val)
{
    m_dTransposeVal = val;
}

void AudioProcessingComponent::setPlaying(bool isPlaying)
{
    m_bPlaying = isPlaying;
}

void AudioProcessingComponent::toggleReverb()
{
    if (m_bReverbOn)
    {
        m_bReverbOn = false;
    }
    else
    {
        m_bReverbOn = true;
    }
}

void AudioProcessingComponent::setSampleRate(float newSampRate)
{
    m_fOutputSampRate = newSampRate;
}

void AudioProcessingComponent::setSource(int source)
{
    if (source == 1)
    {
        m_kSource = Source::karplus;
    }
    else if (source == 2)
    {
        m_kSource = Source::sine;
    }
    else if (source == 3)
    {
        m_kSource = Source::square;
    }
    else if (source == 4)
    {
        m_kSource = Source::triangle;
    }
    else
    {
        m_kSource = Source::sine;
    }
}

void AudioProcessingComponent::setNumHarmonics(float numHarm)
{
    m_fNumHarmonics = numHarm;
}

void AudioProcessingComponent::setLPFCutoff(float frq)
{
    m_fLpfCutoff = frq;
}

void AudioProcessingComponent::setCombFilterVal(int val)
{
    m_iCombFilterVal = val;
}

void AudioProcessingComponent::setFlangerFrq(float frq)
{
    m_fFlangerFrq = frq;
}

void AudioProcessingComponent::setChorusFrq(float frq)
{
    m_fChorusFrq = frq;
}

void AudioProcessingComponent::setVibratoFrq(float frq)
{
    m_fVibratoFrq = frq;
}

void AudioProcessingComponent::setBitDepth(float newBitDepth)
{
    m_fOutputBitDepth = newBitDepth;
}

void AudioProcessingComponent::setDither(bool enableDither)
{
    m_bDitherOn = enableDither;
}

void AudioProcessingComponent::changeSampleRate(float* pfAudio, int numSamples)
{
    //first check for default/no change case  (48k)
    if (m_fOutputSampRate == 48000.0) { return; }

    else
    {
        //apply anti-aliasing filter
        antiAlias.reset();
        antiAlias.setCoefficients(juce::IIRCoefficients::makeLowPass(m_fSampleRate, m_fOutputSampRate * 0.5));
        antiAlias.processSamples(pfAudio, numSamples);
        
        float samplesPerHold = m_fSampleRate / m_fOutputSampRate;

        // cubic interpolation on an arbitrary interval


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

void AudioProcessingComponent::changeBitDepth(float* pfAudio, int numSamples)
{
    //first check for default/no change case  (32 bit / float)
    if (m_fOutputBitDepth == 32.0) { return; }

    else
    {
        float M = pow(2, m_fOutputBitDepth); //number of steps
        float delta = 2.0 / M; //step size
        
        if (m_bDitherOn) //if dither is on...
        {
            for (int i = 0; i < numSamples; i++) //quantize block
            {
                float r1 = random.nextFloat(); //generate random sample
                float a = -1.0; //triangle noise bounds
                float b = 1.0;
                float r = 0.0; //random sample
                if (r1 < 0.5) {
                    r = a + sqrt(r1 * (b - a)*(0.0 - a));
                }
                else {
                    r = b - sqrt((1 - r1) * (b - a) * b);
                }

                float rdelta = 2.0 / (pow(2, 32.0 - m_fOutputBitDepth)); //step size for noise bit depth
                r = (floor(r / rdelta) * rdelta) + (rdelta * 0.5); //get r to the right bit size

                pfAudio[i] = pfAudio[i] + r; //add noise to the current sample
                pfAudio[i] = (floor(pfAudio[i] / delta) * delta) + (delta * 0.5); //mid rise
                //pfAudio[i] = (floor(pfAudio[i] / delta) * delta); //mid tread
            }
        }
        else
        {
            for (int i = 0; i < numSamples; i++) //quantize block
            {
                pfAudio[i] = (floor(pfAudio[i] / delta) * delta) + (delta * 0.5); //mid rise
                //pfAudio[i] = (floor(pfAudio[i] / delta) * delta); //mid tread
            }
        }
    }
}

void AudioProcessingComponent::applyReverb(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples)
{
    revrb->renderNextSubBlock(outputAudio, startSample, numSamples);
}

void AudioProcessingComponent::applyMovingAverageFilter(float* ppfInputBuffer, int iBuffSize, float fcutoff, float fGain)
{
    filt->processMovingAvgFilt(ppfInputBuffer, ppfInputBuffer, iBuffSize, m_fLpfCutoff, 0.9); //LP Filter
}

void AudioProcessingComponent::applyCombFilter()
{

}

void AudioProcessingComponent::applyFlanger()
{

}

void AudioProcessingComponent::applyChorus()
{

}

void AudioProcessingComponent::applyVibrato()
{

}

void AudioProcessingComponent::setEffect(AudioProcessingComponent::Effects& effect, int val)
{
    switch (val)
    {
    case 1:
        effect = Effects::none;
        break;
    case 2:
        effect = Effects::lpf;
        break;
    case 3:
        effect = Effects::reverb;
        break;
    case 4:
        effect = Effects::comb;
        break;
    case 5:
        effect = Effects::flanger;
        break;
    case 6:
        effect = Effects::chorus;
        break;
    case 7:
        effect = Effects::vibrato;
        break;
    default:
        effect = Effects::none;
        break;

    }
}

void AudioProcessingComponent::ModuleManager(Effects* effects, juce::AudioBuffer<float>& outputAudio, int startSample, 
                                                int numSamples, float* ppfInputBuffer, float fcutoff, float fGain)
{
    for (int i = 0; i < 6; i++)
    {
        switch (effects[i])
        {
        case Effects::none:
            break;
        case Effects::reverb:
            applyReverb(outputAudio, startSample, numSamples);
            break;
        case Effects::lpf:
            applyMovingAverageFilter(ppfInputBuffer, numSamples, fcutoff, fGain);
            break;
        case Effects::comb:
            applyCombFilter();
            break;
        case Effects::flanger:
            applyFlanger();
            break;
        case Effects::chorus:
            applyChorus();
            break;
        case Effects::vibrato:
            applyVibrato();
            break;
        default:
            break;
        }
    }
}
