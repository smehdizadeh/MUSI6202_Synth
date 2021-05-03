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
#include <typeinfo> //library needed to check if user input is of certain type

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

    void sourceChanged(); //Handles changes to source
    bool keyPressed(const juce::KeyPress&, juce::Component*) override; // Used to detect key presses
    bool keyStateChanged(bool, Component*) override; //Used to detect key releases

    void samplerateChanged(); //handles changes to the sample rate selection menu

    enum Note
    {
        c = 90,
        cs = 83,
        d = 88,
        ds = 68,
        e = 67,
        f = 86,
        fs = 71,
        g = 66,
        gs = 72,
        a = 78,
        as = 74,
        b = 77,
        cOct = 44,
        csOct = 76,
        dOct = 46,
        dsOct = 59,
        eOct = 47
    };

    enum Tranpositions
    {
        downTwoOct = 49,
        downOneOct,
        level,
        upOneOct,
        upTwoOct,
        numlevels = 5
    };

private:
    void ToggleSynth();
    void GetKey(int);
    //==========================================================================
    juce::Slider lpfCutoff; //Slider for LPF cutoff frequency
    
    juce::Label selNumHarms; //Input box for specifying number of harmonics in additive synthesis

    //for configuring sample rate
    juce::ComboBox samplerateMenu;
    juce::Label chooseSampRate{ {}, "Select output sample rate..." };

    //for configuring source
    juce::ComboBox sourceMenu; //Menu to change the source
    juce::Label chooseSource{ {}, "Select Source..." };

    juce::Label lpfCutoffLabel{ {}, "LPF Cutoff Frequency Slider" };
    juce::Label playDirections{ {}, "Play the synth with the bottom two rows of letter keys! Use the number keys 1-5 at the top to change the octave!" };
    juce::Label reverbDirections{ {}, "Press the tab key to toggle reverb on and off!" };
    
    AudioProcessingComponent& apc;

    juce::KeyPress key; // UI Keyboard presses
    bool m_bTabReleased; // Was tab released?
    bool m_bInitialized; // Was the source button pressed?
    bool m_bNotePlayed; // Was a note played? 
    bool m_bTabPressed;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUIComponent)
};