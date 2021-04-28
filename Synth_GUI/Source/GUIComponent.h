/*
  ==============================================================================
    GUIComponent.h
    Created: 31 Mar 2021 11:53:41am
    Author:  sophi
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioProcessingComponent.h"
#include <iostream>

//==============================================================================
/*
*/
class GUIComponent : public juce::Component, public juce::KeyListener
{
public:
    //==========================================================================
    GUIComponent(AudioProcessingComponent&);
    ~GUIComponent() override;

    //==========================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void sourceBtnClicked(); // Source button clicked. Cycle thru possible synth sources
    bool keyPressed(const juce::KeyPress&, juce::Component*) override; // Used to detect key presses
    bool keyStateChanged(bool, Component*) override; //Used to detect key releases

    void samplerateChanged(); //handles changes to the sample rate selection menu

private:
    //==========================================================================
    juce::Slider dummySlider; //doesn't do anything, just to test the gui
    juce::TextButton sourceBtn; //Button to change the source
    
    //for configuring sample rate
    juce::Label chooseSampRate{ {}, "Select output sample rate..." };
    juce::ComboBox samplerateMenu;
    AudioProcessingComponent& apc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUIComponent)
};