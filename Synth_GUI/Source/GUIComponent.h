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

    enum class Note
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

    enum class Tranpositions
    {
        downTwoOct = 49,
        downOneOct,
        level,
        upOneOct,
        upTwoOct,
        numlevels = 5
    };

    enum class EffectGuis
    {
        lpfGui = 54,
        combGui,
        flangerGui,
        chorusGui,
        vibratoGui = 48,
        numGuis = 5
    };

private:
    void ToggleSynth();
    void Start();
    void ToggleLpfGui();
    void ToggleReverbGui();
    void ToggleCombGui();
    void ToggleFlangerGui();
    void ToggleChorusGui();
    void ToggleVibratoGui();
    void UntoggleAllGuis();

    //========================================================================== 
    
    //Start Button
    juce::TextButton startButton;

    //for configuring sample rate
    juce::ComboBox samplerateMenu;
    juce::Label chooseSampRate{ {}, "Select output sample rate..." };

    //for configuring source
    juce::ComboBox sourceMenu; //Menu to change the source
    juce::Label chooseSource{ {}, "Select Source..." };

    //for configuring the number of harmonics
    juce::Slider numHarms; //slider for specifying number of harmonics in additive synthesis
    juce::Label numHarmsLabel{ {}, "Number of Harmonics" };

    //for LPF
    juce::Label lpfCutoffLabel{ {}, "LPF Cutoff Frequency" };
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
    juce::Label helpText{ {}, "Play the synth with the bottom two rows of letter keys. Use the number keys 1 - 5 at the top to change the octave. Use the number keys 6-9 and 0 to toggle GUIs for the Effects. (6: LPF, 7: Comb, 8: Flanger, 9: Chorus, 0: Vibrato"};
    bool m_bHelpTextOn; //Is the help text visible?

    //for signal pipeline
    juce::ComboBox firstEffect;
    juce::Label firstEffectLabel{ {}, "First Effect" };
    juce::ComboBox secondEffect;
    juce::Label secondEffectLabel{ {}, "Second Effect" };
    juce::ComboBox thirdEffect;
    juce::Label thirdEffectLabel{ {}, "Third Effect" };
    juce::ComboBox fourthEffect;
    juce::Label fourthEffectLabel{ {}, "Fourth Effect" };
    juce::ComboBox fifthEffect;
    juce::Label fifthEffectLabel{ {}, "Fifth Effect" };
    juce::ComboBox sixthEffect;
    juce::Label sixthEffectLabel{ {}, "Sixth Effect" };
   
    AudioProcessingComponent& apc; //Audio Processing Component
    juce::KeyPress key; // UI Keyboard presses

    //UI Component locations
    int m_iX;
    int m_iY;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUIComponent)
};