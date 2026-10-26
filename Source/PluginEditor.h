/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AdsrComponent.h"
#include "OscComponent.h"
#include "FilterComponent.h"

//==============================================================================
/**
*/
class CSC475SynthesizerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CSC475SynthesizerAudioProcessorEditor (CSC475SynthesizerAudioProcessor&);
    ~CSC475SynthesizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CSC475SynthesizerAudioProcessor& audioProcessor;
    OscComponent osc;
    AdsrComponent adsr;
    FilterComponent filter;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CSC475SynthesizerAudioProcessorEditor)
};
