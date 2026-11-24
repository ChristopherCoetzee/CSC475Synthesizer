#include "FilterComponent.h"
#include "KittyFont.h"
#include <cmath>

namespace
{
    constexpr int panelPadding = 6;
    constexpr int titleHeight = 24;
    constexpr int labelHeight = 19;
    constexpr int comboHeight = 20;

    constexpr int sectionGap = 3;
    constexpr int controlGap = 6;

    constexpr int textBoxWidth = 72;
    constexpr int textBoxHeight = 20;
    constexpr int borderWidth = 1;

    constexpr float titleFontSize = 18.0f;
    constexpr float labelFontSize = 15.0f;

    constexpr float knobDrawingPadding = 10.0f;
    constexpr float markerOffset = 4.0f;
    constexpr float markerRadius = 3.0f;
}

FilterComponent::FilterComponent (
    juce::AudioProcessorValueTreeState& apvts,
    juce::String selectorId,
    juce::String frequencyId,
    juce::String amountId)
{
    filterTypeSelector.addItemList (
        juce::StringArray {
            "Low-Pass",
            "Band-Pass",
            "High-Pass"
        },
        1);

    filterTypeSelector.setName ("Filter Type");
    addAndMakeVisible (filterTypeSelector);

    filterTypeSelectorAttachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::ComboBoxAttachment>
            (apvts, selectorId, filterTypeSelector);
    
    // Hide the old dropdown, but keep it connected
    filterTypeSelector.setVisible(false);

    // Set the button labels
    lowPassButton.setButtonText("Low-Pass");
    bandPassButton.setButtonText("Band-Pass");
    highPassButton.setButtonText("High-Pass");
    
    // Orange filter selection buttons
    const auto orange = juce::Colour(0xfff0a35a);

    lowPassButton.setAccentColour(orange);
    bandPassButton.setAccentColour(orange);
    highPassButton.setAccentColour(orange);

    // Make the buttons visible
    addAndMakeVisible(lowPassButton);
    addAndMakeVisible(bandPassButton);
    addAndMakeVisible(highPassButton);

    // Clicking a button selects the filter type
    lowPassButton.onClick = [this]()
    {
        filterTypeSelector.setSelectedId(
            1, juce::sendNotificationSync);
    };

    bandPassButton.onClick = [this]()
    {
        filterTypeSelector.setSelectedId(
            2, juce::sendNotificationSync);
    };

    highPassButton.onClick = [this]()
    {
        filterTypeSelector.setSelectedId(
            3, juce::sendNotificationSync);
    };

    // Update the highlighted selection
    filterTypeSelector.onChange = [this]()
    {
        updateFilterButtons();
    };

    updateFilterButtons();


    filterSelectorLabel.setColour (
        juce::Label::textColourId,
        juce::Colour(0xff604d60));

    filterSelectorLabel.setFont (
        juce::Font (juce::FontOptions (labelFontSize)));

    filterSelectorLabel.setJustificationType (
        juce::Justification::centredLeft);

    addAndMakeVisible (filterSelectorLabel);

    setSliderAndLabel (
        filterFrequencySlider,
        filterFrequencyLabel,
        apvts,
        frequencyId,
        filterFrequencyAttachment);
    
    // Apply orange retro knobs
    filterFrequencySlider.setLookAndFeel(&filterKnobLookAndFeel);
    filterResonanceSlider.setLookAndFeel(&filterKnobLookAndFeel);
    
    setSliderAndLabel (
        filterResonanceSlider,
        filterResonanceLabel,
        apvts,
        amountId,
        filterResonanceAttachment);
}
void FilterComponent::updateFilterButtons()
{
    const int selected = filterTypeSelector.getSelectedId();

    lowPassButton.setToggleState(
        selected == 1, juce::dontSendNotification);

    bandPassButton.setToggleState(
        selected == 2, juce::dontSendNotification);

    highPassButton.setToggleState(
        selected == 3, juce::dontSendNotification);
}

FilterComponent::~FilterComponent()
{
    filterFrequencySlider.setLookAndFeel(nullptr);
    filterResonanceSlider.setLookAndFeel(nullptr);
}

void FilterComponent::paint (juce::Graphics& g)
{
    // Warm white paper background
    g.fillAll(juce::Colour(0xfffffdf6));

    // Crayon orange border
    g.setColour(juce::Colour(0xfff0a35a));
    g.drawRect(getLocalBounds(), 2);

    // Soft dark text
    g.setColour(juce::Colour(0xffb56f33));
    g.setFont(KittyFont::make(titleFontSize));

    g.drawFittedText(
        "FILTER",
        getLocalBounds()
            .reduced(panelPadding)
            .removeFromTop(titleHeight),
        juce::Justification::centredLeft,
        1);
}


void FilterComponent::resized()
{
    auto content = getLocalBounds().reduced(panelPadding);

    // Space for the Filter title
    content.removeFromTop(titleHeight);

    // Hide old dropdown and label
    filterSelectorLabel.setVisible(false);
    filterTypeSelector.setVisible(false);
    filterTypeSelector.setBounds(0, 0, 0, 0);

    // Left column: filter selection buttons
    auto filterArea = content.removeFromLeft(108);

    content.removeFromLeft(controlGap);

    const int rowHeight = filterArea.getHeight() / 3;

    lowPassButton.setBounds(
        filterArea.removeFromTop(rowHeight));

    bandPassButton.setBounds(
        filterArea.removeFromTop(rowHeight));

    highPassButton.setBounds(filterArea);

    // Right columns: Filter knobs
    const int knobWidth =
        (content.getWidth() - controlGap) / 2;

    auto frequencyArea =
        content.removeFromLeft(knobWidth);

    content.removeFromLeft(controlGap);

    filterFrequencyLabel.setBounds(
        frequencyArea.removeFromTop(labelHeight));

    filterFrequencySlider.setBounds(frequencyArea);

    filterResonanceLabel.setBounds(
        content.removeFromTop(labelHeight));

    filterResonanceSlider.setBounds(content);
}


void FilterComponent::setSliderAndLabel (
    juce::Slider& slider,
    juce::Label& label,
    juce::AudioProcessorValueTreeState& apvts,
    juce::String paramId,
    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment>&
        attachment)
{
    slider.setSliderStyle (
        juce::Slider::RotaryHorizontalVerticalDrag);

    slider.setTextBoxStyle (
        juce::Slider::TextBoxBelow,
        true,
        textBoxWidth,
        textBoxHeight);

    slider.setName (label.getText());
    // Orange filter knobs and readable values
    const auto orange = juce::Colour(0xfff0a35a);
    const auto darkOrange = juce::Colour(0xff9d632d);

    slider.setColour(
        juce::Slider::rotarySliderFillColourId, orange);

    slider.setColour(
        juce::Slider::rotarySliderOutlineColourId,
        orange.withAlpha(0.40f));

    slider.setColour(
        juce::Slider::thumbColourId, darkOrange);

    slider.setColour(
        juce::Slider::textBoxTextColourId, darkOrange);

    slider.setColour(
        juce::Slider::textBoxBackgroundColourId,
        juce::Colour(0xfffffdf6));

    slider.setColour(
        juce::Slider::textBoxOutlineColourId,
        darkOrange.withAlpha(0.65f));

    addAndMakeVisible (slider);

    attachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::SliderAttachment>
            (apvts, paramId, slider);

    label.setColour (
        juce::Label::textColourId,
        juce::Colour(0xff9d632d));

    label.setFont(KittyFont::make(labelFontSize));

    label.setJustificationType (
        juce::Justification::centred);

    addAndMakeVisible (label);
}

void FilterComponent::setEffectiveResonance(float resonance)
{
    effectiveResonance = resonance;
    hasEffectiveResonance = true;

    repaint(filterResonanceSlider.getBounds());
}

void FilterComponent::setEffectiveFilterFrequency(float frequency)
{
    effectiveFilterFrequency = frequency;
    hasEffectiveFilterFrequency = true;
    repaint(filterFrequencySlider.getBounds());
}

void FilterComponent::paintOverChildren(juce::Graphics& g)
{
    // Shared drawing code for either knob.
    const auto drawMarker = [&g](
        juce::Slider& slider,
        float effectiveValue)
    {
        const auto layout =
            slider.getLookAndFeel().getSliderLayout(slider);

        const auto knobArea = layout.sliderBounds.toFloat();

        const float centreX =
            static_cast<float>(slider.getX())
            + knobArea.getCentreX();

        const float centreY =
            static_cast<float>(slider.getY())
            + knobArea.getCentreY();

        const float knobDiameter = juce::jmin(
            56.0f,
            juce::jmin(knobArea.getWidth(), knobArea.getHeight()) - 4.0f);

        const float radius = knobDiameter * 0.5f + 4.0f;

        if (radius <= markerRadius)
            return;

        const double limitedValue = juce::jlimit(
            slider.getMinimum(),
            slider.getMaximum(),
            static_cast<double>(effectiveValue));

        // Use the knob's own scale, including its frequency skew.
        const float position = static_cast<float>(
            slider.valueToProportionOfLength(limitedValue));

        const auto rotary = slider.getRotaryParameters();

        const float angle =
            rotary.startAngleRadians
            + position
                * (rotary.endAngleRadians - rotary.startAngleRadians);

        const float markerX =
            centreX + std::sin(angle) * radius;

        const float markerY =
            centreY - std::cos(angle) * radius;

        g.setColour(juce::Colour(0xfff0a35a));

        g.fillEllipse(
            markerX - markerRadius,
            markerY - markerRadius,
            markerRadius * 2.0f,
            markerRadius * 2.0f);
    };

    if (hasEffectiveFilterFrequency)
        drawMarker(filterFrequencySlider, effectiveFilterFrequency);

    if (hasEffectiveResonance)
        drawMarker(filterResonanceSlider, effectiveResonance);
}
