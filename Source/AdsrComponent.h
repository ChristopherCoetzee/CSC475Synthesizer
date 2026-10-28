#pragma once

#include <JuceHeader.h>

class AdsrComponent : public juce::Component
{
public:
    AdsrComponent (
        juce::String name,
        juce::AudioProcessorValueTreeState& apvts,
        juce::String attackId,
        juce::String decayId,
        juce::String sustainId,
        juce::String releaseId);

    ~AdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Label attackLabel { "Attack", "Attack" };
    juce::Label decayLabel { "Decay", "Decay" };
    juce::Label sustainLabel { "Sustain", "Sustain" };
    juce::Label releaseLabel { "Release", "Release" };

    juce::String componentName;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>
        attackAttachment;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>
        decayAttachment;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>
        sustainAttachment;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>
        releaseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
