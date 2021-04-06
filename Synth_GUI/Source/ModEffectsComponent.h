/*
  ==============================================================================

    ModEffectsComponent.h
    Created: 6 Apr 2021 10:23:51am
    Author:  sophi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ModEffectsComponent  : public juce::Component
{
public:
    //=========================================================================
    ModEffectsComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~ModEffectsComponent() override
    {
    }

    //========================================================================
    void processFlanger()
    {
        // FLANGER EFFECT W/ FEEDBACK GOES HERE
    }

    void processVibrato()
    {
        // VIBRATO EFFECT GOES HERE
    }

private:
    //=======================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModEffectsComponent)
};


//===========================================================================
class LFOComponent : public juce::Component
{
public:
    //=======================================================================
    LFOComponent()
    {

    }

    ~LFOComponent() override
    {

    }

    //======================================================================
    // IMPLEMENTATION GOES HERE

private:
    //======================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};