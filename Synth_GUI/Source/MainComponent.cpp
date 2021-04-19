#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    //Child components:
    gui = new GUIComponent(apc);
    addAndMakeVisible(gui);
    addAndMakeVisible(apc);
    setSize (700, 700);
}

MainComponent::~MainComponent()
{
    delete gui;
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    gui->setBounds(getLocalBounds());
}
