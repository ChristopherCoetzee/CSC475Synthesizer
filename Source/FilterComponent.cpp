#include "FilterComponent.h"
#include <cmath>

namespace
{
    constexpr int panelPadding = 12;
    constexpr int titleHeight = 26;
    constexpr int labelHeight = 22;
    constexpr int comboHeight = 30;

    constexpr int sectionGap = 12;
    constexpr int controlGap = 10;

    constexpr int textBoxWidth = 72;
    constexpr int textBoxHeight = 26;
    constexpr int borderWidth = 1;

    constexpr float titleFontSize = 17.0f;
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

    filterSelectorLabel.setColour (
        juce::Label::textColourId,
        juce::Colours::white);

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

    setSliderAndLabel (
        filterResonanceSlider,
        filterResonanceLabel,
        apvts,
        amountId,
        filterResonanceAttachment);
}

FilterComponent::~FilterComponent() = default;

void FilterComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::darkgrey);
    g.drawRect (getLocalBounds(), borderWidth);

    g.setColour (juce::Colours::white);
    g.setFont (
        juce::Font (juce::FontOptions (titleFontSize)));

    g.drawText (
        "Filter",
        getLocalBounds()
            .reduced (panelPadding)
            .removeFromTop (titleHeight),
        juce::Justification::centredLeft);
}

void FilterComponent::resized()
{
    auto content = getLocalBounds().reduced (panelPadding);

    content.removeFromTop (titleHeight);

    filterSelectorLabel.setBounds (
        content.removeFromTop (labelHeight));

    filterTypeSelector.setBounds (
        content.removeFromTop (comboHeight));

    content.removeFromTop (sectionGap);

    const auto controlWidth =
        (content.getWidth() - controlGap) / 2;

    auto frequencyArea = content.removeFromLeft (controlWidth);
    content.removeFromLeft (controlGap);

    filterFrequencyLabel.setBounds (
        frequencyArea.removeFromTop (labelHeight));

    filterFrequencySlider.setBounds (frequencyArea);

    filterResonanceLabel.setBounds (
        content.removeFromTop (labelHeight));

    filterResonanceSlider.setBounds (content);
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

    addAndMakeVisible (slider);

    attachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::SliderAttachment>
            (apvts, paramId, slider);

    label.setColour (
        juce::Label::textColourId,
        juce::Colours::white);

    label.setFont (
        juce::Font (juce::FontOptions (labelFontSize)));

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

void FilterComponent::paintOverChildren(juce::Graphics& g)
{
    if (!hasEffectiveResonance)
        return;

    // Find the area occupied by the resonance knob.
    const auto layout =
        filterResonanceSlider.getLookAndFeel()
            .getSliderLayout(filterResonanceSlider);

    const auto knobArea = layout.sliderBounds.toFloat();

    const float centreX =
        static_cast<float>(filterResonanceSlider.getX())
        + knobArea.getCentreX();

    const float centreY =
        static_cast<float>(filterResonanceSlider.getY())
        + knobArea.getCentreY();


    const float radius =
        juce::jmin(knobArea.getWidth(), knobArea.getHeight()) * 0.5f
        - knobDrawingPadding
        + markerOffset;

    if (radius <= markerRadius)
        return;

    // Make sure the displayed resonance stays within
    // the knob's allowed range.
    const double limitedResonance = juce::jlimit(
        filterResonanceSlider.getMinimum(),
        filterResonanceSlider.getMaximum(),
        static_cast<double>(effectiveResonance));

    // Convert resonance into a position around the knob.
    const float position = static_cast<float>(
        filterResonanceSlider.valueToProportionOfLength(
            limitedResonance));

    const auto rotary =
        filterResonanceSlider.getRotaryParameters();

    const float angle =
        rotary.startAngleRadians
        + position * (rotary.endAngleRadians
                      - rotary.startAngleRadians);

    // Calculate the purple ball's coordinates.
    const float markerX =
        centreX + std::sin(angle) * radius;

    const float markerY =
        centreY - std::cos(angle) * radius;

    // Draw the purple ball.
    g.setColour(juce::Colour(0xffc084fc));

    g.fillEllipse(
        markerX - markerRadius,
        markerY - markerRadius,
        markerRadius * 2.0f,
        markerRadius * 2.0f);
}
