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
    void sourceBtnClicked();
    bool keyPressed(const juce::KeyPress&, juce::Component*) override;
    bool keyStateChanged(bool, Component*) override;


private:
    //==========================================================================
    juce::Slider dummySlider; //doesn't do anything, just to test the gui
    juce::TextButton sourceBtn;
    AudioProcessingComponent& apc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUIComponent)
};