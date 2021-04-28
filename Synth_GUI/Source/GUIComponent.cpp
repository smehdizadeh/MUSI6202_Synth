/*
  ==============================================================================
    GUIComponent.cpp
    Created: 31 Mar 2021 11:53:41am
    Author:  sophi
  ==============================================================================
*/

#include <JuceHeader.h>
#include "GUIComponent.h"
#include <windows.h> //For debug macro
#include <debugapi.h> //for debug macro

//Use DBOUT(arg) to print to the VS console
#define DBOUT(s)            \
{                             \
std::ostringstream os_;    \
os_ << s;                   \
OutputDebugString(os_.str().c_str()); \
OutputDebugString("\n"); \
}

//==============================================================================
GUIComponent::GUIComponent(AudioProcessingComponent& c) : apc(c) //this is how we get the GUI to communicate with the APC
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(dummySlider);
    addAndMakeVisible(sourceBtn);
    sourceBtn.setButtonText("Square Wave");
    sourceBtn.onClick = [this] { sourceBtnClicked(); };
    dummySlider.setRange(0, 100); //doesn't do anything, just to test the gui

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
    g.drawText("GUIComponent", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}

void GUIComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    chooseSampRate.setBounds(10, 10, getWidth() - 20, 20);
    samplerateMenu.setBounds(10, 40, 100, 20);
    sourceBtn.setBounds(30, 70, getWidth() - 60, 20);

    dummySlider.setBounds(10, 100, getWidth() - 20, 20);
}

bool GUIComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    //DBOUT(key.getKeyCode());
    
    apc.GetKey(key.getKeyCode());
    return true;
}

bool GUIComponent::keyStateChanged(bool isKeyDown, juce::Component* originatingComponent)
{
    if (!isKeyDown)
    {
        apc.ToggleSynth();
    }
    return true;
}

void GUIComponent::sourceBtnClicked()
{
    if (sourceBtn.getButtonText() == "Square Wave")
    {
        sourceBtn.setButtonText("Karplus Strong");
    }
    else if (sourceBtn.getButtonText() == "Karplus Strong")
    {
        sourceBtn.setButtonText("Triangle Wave");
    }
    else if (sourceBtn.getButtonText() == "Triangle Wave")
    {
        sourceBtn.setButtonText("Square Wave");
    }
    else
    {
        sourceBtn.setButtonText("Error");
    }

    apc.NextSource();
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