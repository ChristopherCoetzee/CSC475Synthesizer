/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AdsrComponent.h"

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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it
    
    juce::ComboBox oscSelector;
    
    CSC475SynthesizerAudioProcessor& audioProcessor;
    
    AdsrComponent adsr;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CSC475SynthesizerAudioProcessorEditor)
};
