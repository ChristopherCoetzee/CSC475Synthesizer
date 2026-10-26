/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CSC475SynthesizerAudioProcessorEditor::CSC475SynthesizerAudioProcessorEditor (CSC475SynthesizerAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , audioProcessor (p)
    , osc (audioProcessor.apvts, "OSC1WAVETYPE", "FMFREQUENCY", "FMDEPTH")
    , adsr(audioProcessor.apvts)
    , filter(audioProcessor.apvts,"FILTERTYPE", "FILTERFREQUENCY", "FILTERRESONANCE")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
    
    
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
    osc.setBounds(10, 10, 180,200 );
    filter.setBounds(10, 250, 180, 200);
}
