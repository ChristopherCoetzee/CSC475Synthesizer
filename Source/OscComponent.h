#pragma once

#include <JuceHeader.h>

class OscComponent : public juce::Component
{
public:
    OscComponent (
        juce::AudioProcessorValueTreeState& apvts,
        juce::String selectorId,
        juce::String frequencyId,
        juce::String amountId);

    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setEffectiveFmDepth(float depth);
    void paintOverChildren(juce::Graphics& g) override;

private:
    juce::ComboBox oscWaveSelector;

    juce::Slider fmFrequencySlider;
    juce::Slider fmDepthSlider;

    juce::Label waveTypeLabel { "Wave Type", "Wave Type" };
    juce::Label fmFrequencyLabel { "FM Frequency", "FM Frequency" };
    juce::Label fmDepthLabel { "FM Depth", "FM Depth" };

    // Attachments are destroyed before their controls.
    std::unique_ptr<
        juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        oscillatorWaveSelectorAttachment;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>
        fmFrequencyAttachment;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>
        fmDepthAttachment;

    void setSliderAndLabel (
        juce::Slider& slider,
        juce::Label& label,
        juce::AudioProcessorValueTreeState& apvts,
        juce::String paramId,
        std::unique_ptr<
            juce::AudioProcessorValueTreeState::SliderAttachment>&
            attachment);
    
    float effectiveFmDepth { 0.0f };
    bool hasEffectiveFmDepth { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
