#include "AdsrComponent.h"
#include <array>

namespace
{
    constexpr int panelPadding = 12;
    constexpr int titleHeight = 26;
    constexpr int titleGap = 8;
    constexpr int labelHeight = 22;

    constexpr int sliderGap = 6;
    constexpr int sliderCount = 4;

    constexpr int textBoxWidth = 60;
    constexpr int textBoxHeight = 26;
    constexpr int borderWidth = 1;

    constexpr float titleFontSize = 17.0f;
    constexpr float labelFontSize = 14.0f;
}

AdsrComponent::AdsrComponent (
    juce::String name,
    juce::AudioProcessorValueTreeState& apvts,
    juce::String attackId,
    juce::String decayId,
    juce::String sustainId,
    juce::String releaseId)
    : componentName (name)
{
    const std::array<juce::Slider*, sliderCount> sliders
    {{
        &attackSlider,
        &decaySlider,
        &sustainSlider,
        &releaseSlider
    }};

    const std::array<juce::Label*, sliderCount> labels
    {{
        &attackLabel,
        &decayLabel,
        &sustainLabel,
        &releaseLabel
    }};

    for (int i = 0; i < sliderCount; ++i)
    {
        auto& slider = *sliders[i];
        auto& label = *labels[i];

        slider.setSliderStyle (
            juce::Slider::LinearVertical);

        slider.setTextBoxStyle (
            juce::Slider::TextBoxBelow,
            true,
            textBoxWidth,
            textBoxHeight);

        slider.setName (
            componentName + " " + label.getText());

        addAndMakeVisible (slider);

        label.setColour (
            juce::Label::textColourId,
            juce::Colours::white);

        label.setFont (
            juce::Font (juce::FontOptions (labelFontSize)));

        label.setJustificationType (
            juce::Justification::centred);

        addAndMakeVisible (label);
    }

    using Attachment =
        juce::AudioProcessorValueTreeState::SliderAttachment;

    attackAttachment =
        std::make_unique<Attachment> (
            apvts, attackId, attackSlider);

    decayAttachment =
        std::make_unique<Attachment> (
            apvts, decayId, decaySlider);

    sustainAttachment =
        std::make_unique<Attachment> (
            apvts, sustainId, sustainSlider);

    releaseAttachment =
        std::make_unique<Attachment> (
            apvts, releaseId, releaseSlider);
}

AdsrComponent::~AdsrComponent() = default;

void AdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::darkgrey);
    g.drawRect (getLocalBounds(), borderWidth);

    g.setColour (juce::Colours::white);
    g.setFont (
        juce::Font (juce::FontOptions (titleFontSize)));

    g.drawText (
        componentName,
        getLocalBounds()
            .reduced (panelPadding)
            .removeFromTop (titleHeight),
        juce::Justification::centredLeft);
}

void AdsrComponent::resized()
{
    auto content = getLocalBounds().reduced (panelPadding);

    content.removeFromTop (titleHeight + titleGap);

    const auto sliderWidth =
        (content.getWidth() - (sliderCount - 1) * sliderGap)
        / sliderCount;

    const std::array<juce::Slider*, sliderCount> sliders
    {{
        &attackSlider,
        &decaySlider,
        &sustainSlider,
        &releaseSlider
    }};

    const std::array<juce::Label*, sliderCount> labels
    {{
        &attackLabel,
        &decayLabel,
        &sustainLabel,
        &releaseLabel
    }};

    for (int i = 0; i < sliderCount; ++i)
    {
        auto column = content.removeFromLeft (sliderWidth);

        labels[i]->setBounds (
            column.removeFromTop (labelHeight));

        sliders[i]->setBounds (column);

        if (i < sliderCount - 1)
            content.removeFromLeft (sliderGap);
    }
}
