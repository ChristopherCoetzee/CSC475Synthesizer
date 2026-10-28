#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AdsrComponent.h"
#include "OscComponent.h"
#include "FilterComponent.h"
#include <array>

class CSC475SynthesizerAudioProcessorEditor
    : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    CSC475SynthesizerAudioProcessorEditor (
        CSC475SynthesizerAudioProcessor&);

    ~CSC475SynthesizerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CSC475SynthesizerAudioProcessor& audioProcessor;

    // Declared before the controls so it outlives them.
    juce::LookAndFeel_V4 blackLookAndFeel;

    OscComponent osc;
    AdsrComponent adsr;
    AdsrComponent modAdsr;
    FilterComponent filter;
    
    juce::AudioVisualiserComponent voiceWaveform { 1 };
    
    static constexpr int waveformReadBufferSize = 4096;
    
    std::array<float, waveformReadBufferSize> waveformReadBuffer {};

    // Reserved UI areas only.
    juce::Rectangle<int> audioVisualBounds;
    juce::Rectangle<int> voiceVisualBounds;
    juce::Rectangle<int> inputControlsBounds;
    juce::Rectangle<int> midiBounds;
    
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (
        CSC475SynthesizerAudioProcessorEditor)
};
