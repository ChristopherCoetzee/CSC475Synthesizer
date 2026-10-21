/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CSC475SynthesizerAudioProcessorEditor::CSC475SynthesizerAudioProcessorEditor (CSC475SynthesizerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), osc (audioProcessor.apvts, "OSC1WAVETYPE"), adsr(audioProcessor.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    
    
};

CSC475SynthesizerAudioProcessorEditor::~CSC475SynthesizerAudioProcessorEditor()
{
}

//==============================================================================
void CSC475SynthesizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::cornflowerblue);

}

void CSC475SynthesizerAudioProcessorEditor::resized()
{
    adsr.setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
    osc.setBounds(10, 10, 100, 40);
}
