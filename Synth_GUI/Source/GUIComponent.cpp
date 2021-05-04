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
    addAndMakeVisible(chooseBitDepth);
    addAndMakeVisible(bitdepthMenu);
    addAndMakeVisible(ditherButton);
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
    addAndMakeVisible(helpText);

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

    //for configuring bit depth
    chooseBitDepth.setFont(juce::Font{ 16.0f });
    bitdepthMenu.addItem("32 bit", 1);
    bitdepthMenu.addItem("24 bit", 2);
    bitdepthMenu.addItem("20 bit", 3);
    bitdepthMenu.addItem("16 bit", 4);
    bitdepthMenu.addItem("8 bit", 5);

    bitdepthMenu.onChange = [this] {bitdepthChanged(); };
    bitdepthMenu.setSelectedId(1); //default 32 bit

    //dither
    ditherButton.setButtonText("Dither");
    ditherButton.setState(juce::Button::ButtonState::buttonNormal);
    ditherButton.onClick = [this] {apc.setDither(ditherButton.getToggleState()); };

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
    lpfCutoff.setNumDecimalPlacesToDisplay(2);
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
    firstEffect.setSelectedId(1); //default none
    firstEffect.onChange = [this] { apc.setEffect(apc.effects[0], firstEffect.getSelectedId()); };

    //second effect
    secondEffectLabel.setFont(juce::Font{ 16.0 });
    secondEffect.addItem("None", 1);
    secondEffect.addItem("LPF", 2);
    secondEffect.addItem("Reverb", 3);
    secondEffect.setSelectedId(1); //default none
    secondEffect.onChange = [this] { apc.setEffect(apc.effects[1], secondEffect.getSelectedId()); };
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

    // sample rate and bit depth
    chooseSampRate.setBounds(10, 10, getWidth() - 20, 20);
    chooseBitDepth.setBounds(getWidth() - 300, 10, getWidth() - 20, 20);
    samplerateMenu.setBounds(10, 40, 100, 20);
    bitdepthMenu.setBounds(getWidth() - 300, 40, 100, 20);
    ditherButton.setBounds(getWidth() - 175, 40, 100, 20);

    // source
    chooseSource.setBounds(10, 70, getWidth() - 20, 20);
    sourceMenu.setBounds(10, 100, 100, 20);

    // low pass filter
    lpfCutoff.setBounds(10, 150, 150, 150);
    lpfCutoffLabel.setBounds(10, 155, getWidth() - 20, 20);

    //comb filter

    //reverb

    //Effect combo box
    firstEffect.setBounds(getWidth()-300, 200, 100, 20);
    firstEffectLabel.setBounds(getWidth() - 300, 230, 100, 20);
    secondEffect.setBounds(getWidth() - 300, 300, 100, 20);
    secondEffectLabel.setBounds(getWidth() - 300, 330, 100, 20);

    // HelpText
    helpText.setBounds(10, getHeight()-200, 400, 200);
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

void GUIComponent::bitdepthChanged()
{
    switch (bitdepthMenu.getSelectedId())
    {
    case 1: //32 bit
        apc.setBitDepth(32.0);
        break;
    case 2: //24 bit
        apc.setBitDepth(24.0);
        break;
    case 3: //20 bit
        apc.setBitDepth(20.0);
        break;
    case 4: //16 bit
        apc.setBitDepth(16.0);
        break;
    case 5: //8 bit
        apc.setBitDepth(8.0);
        break;
    default: //32 bit
        apc.setBitDepth(32.0);
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

