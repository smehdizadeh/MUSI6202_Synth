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


//==============================================================================
GUIComponent::GUIComponent(AudioProcessingComponent& c) : 
    apc(c), //this is how we get the GUI to communicate with the APC
    m_bTabReleased(false),
    m_bTabPressed(false),
    m_bNotePlayed(false),
    m_bInitialized(false)
{
    // LPF cutoff slider
    lpfCutoff.setSliderStyle(juce::Slider::Rotary);
    addAndMakeVisible(lpfCutoff);
    lpfCutoff.setRange(22, 10000);
    lpfCutoff.setValue(3000);
    lpfCutoff.onValueChange = [this]
    {
        apc.SetLPFCutoff(lpfCutoff.getValue());
    };
    

    //Keyboard Listeners
    setWantsKeyboardFocus(true);
    addKeyListener(this);

    //for configuring sample rate
    addAndMakeVisible(chooseSampRate);
    chooseSampRate.setFont(juce::Font{ 16.0f });
    addAndMakeVisible(samplerateMenu);
    samplerateMenu.addItem("48 kHz", 1);
    samplerateMenu.addItem("44.1 kHz", 2);
    samplerateMenu.addItem("22.05 kHz", 3);
    samplerateMenu.addItem("16 kHz", 4);
    samplerateMenu.onChange = [this] {samplerateChanged(); };
    samplerateMenu.setSelectedId(1); //default 48k


    //for configuring source
    addAndMakeVisible(sourceMenu);
    sourceMenu.addItem("Karplus", 1);
    sourceMenu.addItem("Sine", 2);
    sourceMenu.addItem("Square", 3);
    sourceMenu.addItem("Triangle", 4);
    sourceMenu.onChange = [this] {sourceChanged(); };
    sourceMenu.setSelectedId(1); //default karplus Strong
    

    //Slider Labels and directions
    addAndMakeVisible(lpfCutoffLabel);
    lpfCutoffLabel.setFont(juce::Font{ 16.0f });
    //addAndMakeVisible(playDirections);
    playDirections.setFont(juce::Font{ 16.0f });
    playDirections.setVisible(false);
    //addAndMakeVisible(reverbDirections);
    reverbDirections.setFont(juce::Font{ 16.0f });
    reverbDirections.setVisible(false);


    // ==== Still being implemented ==== //

    /*addAndMakeVisible(selNumHarms);
    selNumHarms.setEditable(true);
    selNumHarms.onTextChange = [this]
    {
        if (typeid(selNumHarms.getText()).name() == typeid(int).name())
        {
            apc.SetNumHarmonics(selNumHarms.getText());
        }
    };*/
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
    auto border = 4;
    auto area = getLocalBounds();
    auto dialArea = area.removeFromTop(area.getHeight() / 2);

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

    // directions
    playDirections.setBounds(10, 400, getWidth() - 20, 20);
    reverbDirections.setBounds(10, 200, getWidth() - 20, 20);
}

bool GUIComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    if (key == juce::KeyPress::tabKey)
    {
        if (!m_bTabPressed)
        {
            //reverbDirections.setVisible(false);
            m_bTabPressed = true;
        }
        if (m_bTabReleased)
        {
            m_bTabReleased = false;
            apc.ToggleReverb();
            return true;
        }
    }
    else
    {
        if (m_bInitialized)
        {
            //playDirections.setVisible(false);
        }
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
        DBOUT(m_bTabReleased);
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
        apc.SetPlaying(false);
    }
}

void GUIComponent::GetKey(int press)
{
    switch (press)
    {
    case (int)Tranpositions::downTwoOct:
        apc.SetTranspositionVal(-24);
        break;
    case (int)Tranpositions::downOneOct:
        apc.SetTranspositionVal(-12);
        break;
    case (int)Tranpositions::level:
        apc.SetTranspositionVal(0);
        break;
    case (int)Tranpositions::upOneOct:
        apc.SetTranspositionVal(12);
        break;
    case (int)Tranpositions::upTwoOct:
        apc.SetTranspositionVal(24);
        break;
    case (int)Note::c:
        apc.SetFrq(48);
        break;
    case (int)Note::cs:
        apc.SetFrq(49);
        break;
    case (int)Note::d:
        apc.SetFrq(50);
        break;
    case (int)Note::ds:
        apc.SetFrq(51);
        break;
    case (int)Note::e:
        apc.SetFrq(52);
        break;
    case (int)Note::f:
        apc.SetFrq(53);
        break;
    case (int)Note::fs:
        apc.SetFrq(54);
        break;
    case (int)Note::g:
        apc.SetFrq(55);
        break;
    case (int)Note::gs:
        apc.SetFrq(56);
        break;
    case (int)Note::a:
        apc.SetFrq(57);
        break;
    case (int)Note::as:
        apc.SetFrq(58);
        break;
    case (int)Note::b:
        apc.SetFrq(59);
        break;
    case (int)Note::cOct:
        apc.SetFrq(60);
        break;
    case (int)Note::csOct:
        apc.SetFrq(61);
        break;
    case (int)Note::dOct:
        apc.SetFrq(62);
        break;
    case (int)Note::dsOct:
        apc.SetFrq(63);
        break;
    case (int)Note::eOct:
        apc.SetFrq(64);
        break;
    default:
        break;
    }
}

void GUIComponent::sourceChanged()
{
    if (!m_bInitialized)
    {
        playDirections.setVisible(true);
        reverbDirections.setVisible(true);
        m_bInitialized = true;
    }

    switch (sourceMenu.getSelectedId())
    {
    case 1:
        apc.SetSource(1);
        break;
    case 2:
        apc.SetSource(2);
        break;
    case 3:
        apc.SetSource(3);
        break;
    case 4:
        apc.SetSource(4);
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