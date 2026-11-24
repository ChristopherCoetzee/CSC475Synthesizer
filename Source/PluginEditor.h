
#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "AdsrComponent.h"
#include "OscComponent.h"
#include "FilterComponent.h"
#include "SynthScopeComponent.h"

#include <array>

class CSC475SynthesizerAudioProcessorEditor
    : public juce::AudioProcessorEditor,
      private juce::Timer
{
public:
    CSC475SynthesizerAudioProcessorEditor(
        CSC475SynthesizerAudioProcessor&);

    ~CSC475SynthesizerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Audio processor
    CSC475SynthesizerAudioProcessor& audioProcessor;

    // Look and feel
    juce::LookAndFeel_V4 blackLookAndFeel;

    // Synth controls
    OscComponent osc;
    AdsrComponent adsr;
    AdsrComponent modAdsr;
    FilterComponent filter;

    // Live microphone waveform
    juce::AudioVisualiserComponent voiceWaveform { 1 };

    // Custom single-line synth oscilloscope
    SynthScopeComponent synthWaveform;

    // Audio sample buffer
    static constexpr int waveformReadBufferSize = 4096;

    std::array<float, waveformReadBufferSize>
        waveformReadBuffer {};
    // Controls how frequently the synth waveform updates visually
    int scopeRefreshCounter = 0;

    // Cat illustration
    juce::Rectangle<int> catBannerBounds;
    juce::Image catImage;

    // Panel positions
    juce::Rectangle<int> audioVisualBounds;
    juce::Rectangle<int> voiceVisualBounds;
    juce::Rectangle<int> inputControlsBounds;
    juce::Rectangle<int> midiBounds;

    // Update visualizers
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        CSC475SynthesizerAudioProcessorEditor)
};
