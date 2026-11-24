#include "AdsrComponent.h"
#include "KittyFont.h"
#include <array>

namespace
{
    constexpr int panelPadding = 5;
    constexpr int titleHeight = 24;
    constexpr int titleGap = 2;
    constexpr int labelHeight = 19;

    constexpr int sliderGap = 4;
    constexpr int sliderCount = 4;

    constexpr int textBoxWidth = 72;
    constexpr int textBoxHeight = 20;

    constexpr int borderWidth = 1;
    constexpr float titleFontSize = 18.0f;
    constexpr float labelFontSize = 15.0f;
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
    // Blue paws for Amplitude ADSR, green for Modulation ADSR
    pawSliderLookAndFeel.setPawColour(
        componentName == "Amplitude ADSR"
            ? juce::Colour(0xff6faee8)
            : juce::Colour(0xff7fc97f));
    const bool isAmplitude =
        (componentName == "Amplitude ADSR");

    const auto accent = isAmplitude
        ? juce::Colour(0xff6faee8)  // Blue
        : juce::Colour(0xff7fc97f); // Green

    const auto darkAccent = isAmplitude
        ? juce::Colour(0xff3f729e)
        : juce::Colour(0xff3e8249);
    
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
        
        // Apply the cat-paw design
        slider.setLookAndFeel(&pawSliderLookAndFeel);
        
        slider.setName (
            componentName + " " + label.getText());
        
        // Match slider colours to its ADSR section
        slider.setColour(
            juce::Slider::trackColourId, accent);

        slider.setColour(
            juce::Slider::backgroundColourId,
            accent.withAlpha(0.35f));

        slider.setColour(
            juce::Slider::thumbColourId, darkAccent);

        slider.setColour(
            juce::Slider::textBoxTextColourId, darkAccent);

        slider.setColour(
            juce::Slider::textBoxBackgroundColourId,
            juce::Colour(0xfffffdf6));

        slider.setColour(
            juce::Slider::textBoxOutlineColourId,
            darkAccent.withAlpha(0.65f));

        addAndMakeVisible (slider);

        label.setColour (juce::Label::textColourId, darkAccent);

        label.setFont(KittyFont::make(labelFontSize));

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

AdsrComponent::~AdsrComponent()
{
    attackSlider.setLookAndFeel(nullptr);
    decaySlider.setLookAndFeel(nullptr);
    sustainSlider.setLookAndFeel(nullptr);
    releaseSlider.setLookAndFeel(nullptr);
}

void AdsrComponent::paint(juce::Graphics& g)
{
    // Warm paper background
    g.fillAll(juce::Colour(0xfffffdf6));
    
    // Blue Amplitude ADSR, green Modulation ADSR
    const bool isAmplitude =
    (componentName == "Amplitude ADSR");
    
    const auto borderColour = isAmplitude
    ? juce::Colour(0xff6faee8)
    : juce::Colour(0xff7fc97f);
    
    const auto titleColour = isAmplitude
    ? juce::Colour(0xff4e85b8)
    : juce::Colour(0xff4f9550);
    
    g.setColour(borderColour);
    g.drawRect(getLocalBounds(), 2);
    
    // Dark, readable labels
    g.setColour(titleColour);
    
    g.setFont(KittyFont::make(titleFontSize));
    
    g.drawFittedText(
                     componentName.toUpperCase(),
                     getLocalBounds()
                     .reduced(panelPadding)
                     .removeFromTop(titleHeight),
                     juce::Justification::centredLeft,
                     1);
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
