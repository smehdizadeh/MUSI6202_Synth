/*
  ==============================================================================
    GUIComponent.cpp
    Created: 31 Mar 2021 11:53:41am
    Author:  sophi
  ==============================================================================
*/

#include <JuceHeader.h>
#include "GUIComponent.h"
#include <cmath>

void GUIComponent::Start()
{
    addAndMakeVisible(chooseSampRate);
    addAndMakeVisible(samplerateMenu);
    addAndMakeVisible(sourceMenu);
    addAndMakeVisible(numHarmsLabel);
    addAndMakeVisible(numHarms);
    addAndMakeVisible(lpfCutoffLabel);
    addAndMakeVisible(lpfCutoff);
    addAndMakeVisible(combFilterLabel);
    addAndMakeVisible(combFilter);
    addAndMakeVisible(flangerFrqLabel);
    addAndMakeVisible(flangerFrq);
    addAndMakeVisible(chorusFrqLabel);
    addAndMakeVisible(chorusFrq);
    addAndMakeVisible(vibratoFrqLabel);
    addAndMakeVisible(vibratoFrq);
    addAndMakeVisible(firstEffectLabel);
    addAndMakeVisible(firstEffect);
    addAndMakeVisible(secondEffectLabel);
    addAndMakeVisible(secondEffect);
    addAndMakeVisible(thirdEffect);
    addAndMakeVisible(thirdEffectLabel);
    addAndMakeVisible(fourthEffect);
    addAndMakeVisible(fourthEffectLabel);
    addAndMakeVisible(fifthEffect);
    addAndMakeVisible(fifthEffectLabel);
    addAndMakeVisible(sixthEffect);
    addAndMakeVisible(sixthEffectLabel);

    startButton.setVisible(false);
}

//==============================================================================
GUIComponent::GUIComponent(AudioProcessingComponent& c) :
    apc(c), //this is how we get the GUI to communicate with the APC
    m_bTabReleased(false),
    m_bHelpTextOn(false)
{   
    //Start Button
    addAndMakeVisible(startButton);
    startButton.setButtonText("Click Here to Start the Synthesizer!");
    startButton.onClick = [this] { Start(); };

    //Keyboard Listeners
    setWantsKeyboardFocus(true);
    addKeyListener(this);

    //for configuring sample rate
    chooseSampRate.setFont(juce::Font{ 16.0f });
    samplerateMenu.addItem("48 kHz", 1);
    samplerateMenu.addItem("44.1 kHz", 2);
    samplerateMenu.addItem("22.05 kHz", 3);
    samplerateMenu.addItem("16 kHz", 4);
    samplerateMenu.onChange = [this] {samplerateChanged(); };
    samplerateMenu.setSelectedId(1); //default 48k

    //for configuring source
    sourceMenu.addItem("Karplus", 1);
    sourceMenu.addItem("Sine", 2);
    sourceMenu.addItem("Square", 3);
    sourceMenu.addItem("Triangle", 4);
    sourceMenu.onChange = [this] {sourceChanged(); };
    sourceMenu.setSelectedId(1); //default karplus Strong

    //for configuring the number of harmonics
    numHarmsLabel.setFont(juce::Font{ 16.0f });
    numHarms.setSliderStyle(juce::Slider::Rotary);
    numHarms.setRange(1, 200);
    numHarms.setValue(20);
    numHarms.onValueChange = [this] {};
    
    //for configuring LPF
    lpfCutoffLabel.setFont(juce::Font{ 16.0f });
    lpfCutoff.setSliderStyle(juce::Slider::Rotary);
    lpfCutoff.setRange(22, 10000);
    lpfCutoff.setValue(3000);
    lpfCutoff.onValueChange = [this] { apc.setLPFCutoff(lpfCutoff.getValue()); };


    // ===== Range and init value should be changed to something more accurate for comb filter, flanger, chorus, and vibrato ===== //
    //for configuring comb filter
    combFilterLabel.setFont(juce::Font{ 16.0f });
    combFilter.setSliderStyle(juce::Slider::Rotary);
    combFilter.setRange(100, 1000); //Range and init value can and should be changed to something more accurate
    combFilter.setValue(500);
    combFilter.onValueChange = [this] { apc.setCombFilterVal((int)combFilter.getValue()); };

    //for configuring flanger
    flangerFrqLabel.setFont(juce::Font{ 16.0f });
    flangerFrq.setSliderStyle(juce::Slider::Rotary);
    flangerFrq.setRange(100, 1000);
    chorusFrq.setValue(500);
    flangerFrq.onValueChange = [this] { apc.setFlangerFrq(flangerFrq.getValue()); };

    //for configuring chorus
    chorusFrqLabel.setFont(juce::Font{ 16.0f });
    chorusFrq.setSliderStyle(juce::Slider::Rotary);
    chorusFrq.setRange(100, 1000);
    chorusFrq.setValue(500);
    chorusFrq.onValueChange = [this] { apc.setChorusFrq(chorusFrq.getValue()); };

    //for configuring vibrato
    vibratoFrqLabel.setFont(juce::Font{ 16.0f });
    vibratoFrq.setRange(100, 1000);
    vibratoFrq.setValue(500);
    vibratoFrq.onValueChange = [this] { apc.setVibratoFrq(vibratoFrq.getValue()); };

    //for configuring help text
    helpText.setFont(juce::Font{ 16.0f });

    //for configuring signal pipeline
    //first effect
    firstEffectLabel.setFont(juce::Font{ 16.0 });
    firstEffect.addItem("None", 1);
    firstEffect.addItem("LPF", 2);
    firstEffect.addItem("Reverb", 3);
    firstEffect.addItem("Comb Filter", 4);
    firstEffect.addItem("Flanger", 5);
    firstEffect.addItem("Chorus", 6);
    firstEffect.addItem("Vibrato", 7);
    firstEffect.setSelectedId(0); //default none
    firstEffect.onChange = [this] { apc.setEffect(apc.effects[0], firstEffect.getSelectedId()); };

    //second effect
    secondEffectLabel.setFont(juce::Font{ 16.0 });
    secondEffect.addItem("None", 1);
    secondEffect.addItem("LPF", 2);
    secondEffect.addItem("Reverb", 3);
    secondEffect.addItem("Comb Filter", 4);
    secondEffect.addItem("Flanger", 5);
    secondEffect.addItem("Chorus", 6);
    secondEffect.addItem("Vibrato", 7);
    secondEffect.setSelectedId(0); //default none
    secondEffect.onChange = [this] { apc.setEffect(apc.effects[1], secondEffect.getSelectedId()); };

    //third effect
    thirdEffectLabel.setFont(juce::Font{ 16.0 });
    thirdEffect.addItem("None", 1);
    thirdEffect.addItem("LPF", 2);
    thirdEffect.addItem("Reverb", 3);
    thirdEffect.addItem("Comb Filter", 4);
    thirdEffect.addItem("Flanger", 5);
    thirdEffect.addItem("Chorus", 6);
    thirdEffect.addItem("Vibrato", 7);
    thirdEffect.setSelectedId(0); //default none
    thirdEffect.onChange = [this] { apc.setEffect(apc.effects[2], thirdEffect.getSelectedId()); };

    //fourth effect
    fourthEffectLabel.setFont(juce::Font{ 16.0 });
    fourthEffect.addItem("None", 1);
    fourthEffect.addItem("LPF", 2);
    fourthEffect.addItem("Reverb", 3);
    fourthEffect.addItem("Comb Filter", 4);
    fourthEffect.addItem("Flanger", 5);
    fourthEffect.addItem("Chorus", 6);
    fourthEffect.addItem("Vibrato", 7);
    fourthEffect.setSelectedId(0); //default none
    fourthEffect.onChange = [this] { apc.setEffect(apc.effects[3], fourthEffect.getSelectedId()); };

    //fifth effect
    fifthEffectLabel.setFont(juce::Font{ 16.0 });
    fifthEffect.addItem("None", 1);
    fifthEffect.addItem("LPF", 2);
    fifthEffect.addItem("Reverb", 3);
    fifthEffect.addItem("Comb Filter", 4);
    fifthEffect.addItem("Flanger", 5);
    fifthEffect.addItem("Chorus", 6);
    fifthEffect.addItem("Vibrato", 7);
    fifthEffect.setSelectedId(0); //default none
    fifthEffect.onChange = [this] { apc.setEffect(apc.effects[4], fifthEffect.getSelectedId()); };

    //sixth effect
    sixthEffectLabel.setFont(juce::Font{ 16.0 });
    sixthEffect.addItem("None", 1);
    sixthEffect.addItem("LPF", 2);
    sixthEffect.addItem("Reverb", 3);
    sixthEffect.addItem("Comb Filter", 4);
    sixthEffect.addItem("Flanger", 5);
    sixthEffect.addItem("Chorus", 6);
    sixthEffect.addItem("Vibrato", 7);
    sixthEffect.setSelectedId(0); //default none
    sixthEffect.onChange = [this] { apc.setEffect(apc.effects[5], sixthEffect.getSelectedId()); };
}

GUIComponent::~GUIComponent()
{
  
}

//==============================================================================
void GUIComponent::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.
       You should replace everything in this method with your own
       drawing code..
    */
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
}

void GUIComponent::resized() //GUI positions on screen
{
    //start button
    startButton.setBounds(200, 200, 300, 100);

    // sample rate
    chooseSampRate.setBounds(10, 10, getWidth() - 20, 20);
    samplerateMenu.setBounds(10, 40, 100, 20);

    // source
    chooseSource.setBounds(10, 70, getWidth() - 20, 20);
    sourceMenu.setBounds(10, 100, 100, 20);

    // low pass filter
    lpfCutoff.setBounds(0, 150, 150, 150);
    lpfCutoffLabel.setBounds(150, 155, getWidth() - 20, 20);

    //comb filter

    //reverb

    //Effect combo boxes and labels
    firstEffect.setBounds(550, 50, 100, 20);
    firstEffectLabel.setBounds(550, 80, 100, 20);
    secondEffect.setBounds(550, 150, 100, 20);
    secondEffectLabel.setBounds(550, 180, 100, 20);
    thirdEffect.setBounds(550, 250, 100, 20);
    thirdEffectLabel.setBounds(550, 280, 100, 20);
    fourthEffect.setBounds(550, 350, 100, 20);
    fourthEffectLabel.setBounds(550, 380, 100, 20);
    fifthEffect.setBounds(550, 450, 100, 20);
    fifthEffectLabel.setBounds(550, 480, 100, 20);
    sixthEffect.setBounds(550, 550, 100, 20);
    sixthEffectLabel.setBounds(550, 580, 100, 20);

    // HelpText
    helpText.setBounds(10, 400, 400, 200);
}

bool GUIComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    if (key == juce::KeyPress::escapeKey)
    {
        ToggleHelp();
    }

    else
    {
        GetKey(key.getKeyCode());
        return true;
    }
}

bool GUIComponent::keyStateChanged(bool isKeyDown, juce::Component* originatingComponent)
{
    if (!isKeyDown)
    {
        ToggleSynth();
    }
    return true;
}

void GUIComponent::ToggleSynth()   
{
    if (!key.isKeyCurrentlyDown((juce::KeyPress::tabKey)))
    {
        m_bTabReleased = true;
    }

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
    else if ((key.isKeyCurrentlyDown((int)Note::csOct)))
    {

    }
    else if ((key.isKeyCurrentlyDown((int)Note::dOct)))
    {

    }
    else if ((key.isKeyCurrentlyDown((int)Note::dsOct)))
    {

    }
    else if ((key.isKeyCurrentlyDown((int)Note::eOct)))
    {

    }
    else
    {
        apc.setPlaying(false);
    }
}

void GUIComponent::GetKey(int press)
{
    switch (press)
    {
    case (int)Tranpositions::downTwoOct:
        apc.setTranspositionVal(-24);
        break;
    case (int)Tranpositions::downOneOct:
        apc.setTranspositionVal(-12);
        break;
    case (int)Tranpositions::level:
        apc.setTranspositionVal(0);
        break;
    case (int)Tranpositions::upOneOct:
        apc.setTranspositionVal(12);
        break;
    case (int)Tranpositions::upTwoOct:
        apc.setTranspositionVal(24);
        break;
    case (int)Note::c:
        apc.setFrq(48);
        break;
    case (int)Note::cs:
        apc.setFrq(49);
        break;
    case (int)Note::d:
        apc.setFrq(50);
        break;
    case (int)Note::ds:
        apc.setFrq(51);
        break;
    case (int)Note::e:
        apc.setFrq(52);
        break;
    case (int)Note::f:
        apc.setFrq(53);
        break;
    case (int)Note::fs:
        apc.setFrq(54);
        break;
    case (int)Note::g:
        apc.setFrq(55);
        break;
    case (int)Note::gs:
        apc.setFrq(56);
        break;
    case (int)Note::a:
        apc.setFrq(57);
        break;
    case (int)Note::as:
        apc.setFrq(58);
        break;
    case (int)Note::b:
        apc.setFrq(59);
        break;
    case (int)Note::cOct:
        apc.setFrq(60);
        break;
    case (int)Note::csOct:
        apc.setFrq(61);
        break;
    case (int)Note::dOct:
        apc.setFrq(62);
        break;
    case (int)Note::dsOct:
        apc.setFrq(63);
        break;
    case (int)Note::eOct:
        apc.setFrq(64);
        break;
    default:
        break;
    }
}

void GUIComponent::sourceChanged()
{

    switch (sourceMenu.getSelectedId())
    {
    case 1:
        apc.setSource(1);
        break;
    case 2:
        apc.setSource(2);
        break;
    case 3:
        apc.setSource(3);
        break;
    case 4:
        apc.setSource(4);
        break;
    }
}

void GUIComponent::samplerateChanged()
{
    switch (samplerateMenu.getSelectedId())
    {
    case 1: //48k
        apc.setSampleRate(48000.0);
        break;
    case 2: //44.1k
        apc.setSampleRate(44100.0);
        break;
    case 3: //22.05k
        apc.setSampleRate(22050.0);
        break;
    case 4: //16k
        apc.setSampleRate(16000.0);
        break;
    default: //48k
        apc.setSampleRate(48000.0);
        break;
    }
}

void GUIComponent::ToggleHelp()
{
    if (m_bHelpTextOn)
    {
        helpText.setVisible(false);
        m_bHelpTextOn = false;
    }
    else
    {
        helpText.setVisible(true);
        m_bHelpTextOn = true;
    }
}

