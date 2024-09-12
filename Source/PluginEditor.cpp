/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WeirdEffectsAudioProcessorEditor::WeirdEffectsAudioProcessorEditor(WeirdEffectsAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {   
            // Make sure that before the constructor has finished, you've set the
            // editor's size to whatever you need it to be.
            setSize(800, 600);        
}

WeirdEffectsAudioProcessorEditor::~WeirdEffectsAudioProcessorEditor()
{


}

//==============================================================================
void WeirdEffectsAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Audio Volume", 5, 0, getWidth(), 30, juce::Justification::verticallyCentred, 1);

}

void WeirdEffectsAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
