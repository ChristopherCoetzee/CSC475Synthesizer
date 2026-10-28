#pragma once

#include <JuceHeader.h>

class FilterComponent : public juce::Component
{
public:
    FilterComponent (
        juce::AudioProcessorValueTreeState& apvts,
        juce::String selectorId,
        juce::String frequencyId,
        juce::String amountId);

    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox filterTypeSelector;

    juce::Slider filterFrequencySlider;
    juce::Slider filterResonanceSlider;

    juce::Label filterSelectorLabel {
        "Filter Type", "Filter Type"
    };

    juce::Label filterFrequencyLabel {
        "Filter Frequency", "Filter Frequency"
    };

    juce::Label filterResonanceLabel {
        "Filter Resonance", "Filter Resonance"
    };

    // Attachments are destroyed before their controls.
    std::unique_ptr<
        juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        filterTypeSelectorAttachment;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>
        filterFrequencyAttachment;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>
        filterResonanceAttachment;

    void setSliderAndLabel (
        juce::Slider& slider,
        juce::Label& label,
        juce::AudioProcessorValueTreeState& apvts,
        juce::String paramId,
        std::unique_ptr<
            juce::AudioProcessorValueTreeState::SliderAttachment>&
            attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
