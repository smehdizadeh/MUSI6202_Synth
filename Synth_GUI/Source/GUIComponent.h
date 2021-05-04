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

    void bitdepthChanged(); //handles changes to the bit depth selection menu

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
    void ToggleHelp();
    void Start();
    //========================================================================== 
    
    //Start Button
    juce::TextButton startButton;

    //for configuring sample rate
    juce::Label yourSampRate{ {}, "Your output sample rate is..." };
    juce::Label sampRateInstruct{ {}, "Please make sure your system is using the desired sample rate.\n Sample rate can only be changed in your system settings, not within this App.\n If you would like to change your output sample rate, please quit the App before doing so.\n On Windows this can be done by going to Sounds > Playback > (Select your output device) > Properties > Advanced > (Select the desired sample rate under Default Format) > Apply\n This App only supports sample rates 48k and below.\n\n Your current output sample rate is:" };
    juce::Label sampRateDisplay1;
    juce::Label sampRateDisplay2;

    //for configuring bit depth and dithering
    juce::ComboBox bitdepthMenu;
    juce::Label chooseBitDepth{ {}, "Select output bit depth..." };
    juce::ToggleButton ditherButton; //toggle dithering on/off

    //for configuring source
    juce::ComboBox sourceMenu; //Menu to change the source
    juce::Label chooseSource{ {}, "Select Source..." };

    //for configuring the number of harmonics
    juce::Slider numHarms; //slider for specifying number of harmonics in additive synthesis
    juce::Label numHarmsLabel{ {}, "Adjust the number of harmonics" };

    //for LPF
    juce::Label lpfCutoffLabel{ {}, "LPF Cutoff Frequency Slider" };
    juce::Slider lpfCutoff; //Slider for LPF cutoff frequency

    //for comb filter
    juce::Label combFilterLabel{ {}, "Comb Filter Sample Delay" };
    juce::Slider combFilter;

    //for reverb
    bool m_bTabReleased; // Was tab released?

    //for flanger
    juce::Label flangerFrqLabel{ {}, "Flanger Frequency" };
    juce::Slider flangerFrq;

    //for chorus
    juce::Label chorusFrqLabel{ {}, "Chorus Frequency" };
    juce::Slider chorusFrq;

    //for vibrato
    juce::Label vibratoFrqLabel{ {}, "Vibrato Frequency" };
    juce::Slider vibratoFrq;

    //for help text
    juce::Label helpText{ {}, "Play the synth with the bottom two rows of letter keys. Use the number keys 1 - 5 at the top to change the octave. Press escape to toggle these instructions on and off." };
    bool m_bHelpTextOn; //Is the help text visible?

    //for signal pipeline
    juce::ComboBox firstEffect;
    juce::Label firstEffectLabel{ {}, "First Effect" };
    juce::ComboBox secondEffect;
    juce::Label secondEffectLabel{ {}, "Second Effect" };
    
    AudioProcessingComponent& apc; //Audio Processing Component
    juce::KeyPress key; // UI Keyboard presses
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUIComponent)
};