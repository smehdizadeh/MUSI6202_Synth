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

//==============================================================================
/*
*/
class GUIComponent : public juce::Component
{
public:
    //==========================================================================
    GUIComponent(AudioProcessingComponent&);
    ~GUIComponent() override;

    //==========================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void samplerateChanged(); //handles changes to the sample rate selection menu

private:
    //==========================================================================
    juce::Slider dummySlider; //doesn't do anything, just to test the gui
    
    //for configuring sample rate
    juce::Label chooseSampRate{ {}, "Select output sample rate..." };
    juce::ComboBox samplerateMenu;
    AudioProcessingComponent& apc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUIComponent)
};