/*
  ==============================================================================

    GUIComponent.cpp
    Created: 31 Mar 2021 11:53:41am
    Author:  sophi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GUIComponent.h"

//==============================================================================
GUIComponent::GUIComponent(AudioProcessingComponent& c) : apc(c) //this is how we get the GUI to communicate with the APC
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(dummySlider);
    dummySlider.setRange(0, 100); //doesn't do anything, just to test the gui
}

GUIComponent::~GUIComponent()
{
}

//==============================================================================
void GUIComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("GUIComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void GUIComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    dummySlider.setBounds(10, 10, getWidth() - 20, 20);
}