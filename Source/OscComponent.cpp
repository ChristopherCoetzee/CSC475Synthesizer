#include "OscComponent.h"
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

OscComponent::OscComponent (
    juce::AudioProcessorValueTreeState& apvts,
    juce::String selectorId,
    juce::String frequencyId,
    juce::String amountId)
{
    oscWaveSelector.addItemList (
        juce::StringArray { "Sine", "Saw", "Square" },
        1);

    oscWaveSelector.setName ("Wave Type");
    addAndMakeVisible (oscWaveSelector);

    oscillatorWaveSelectorAttachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::ComboBoxAttachment>
            (apvts, selectorId, oscWaveSelector);

    waveTypeLabel.setColour (
        juce::Label::textColourId,
        juce::Colours::white);

    waveTypeLabel.setFont (
        juce::Font (juce::FontOptions (labelFontSize)));

    waveTypeLabel.setJustificationType (
        juce::Justification::centredLeft);

    addAndMakeVisible (waveTypeLabel);

    setSliderAndLabel (
        fmFrequencySlider,
        fmFrequencyLabel,
        apvts,
        frequencyId,
        fmFrequencyAttachment);

    setSliderAndLabel (
        fmDepthSlider,
        fmDepthLabel,
        apvts,
        amountId,
        fmDepthAttachment);
}

OscComponent::~OscComponent() = default;

void OscComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::darkgrey);
    g.drawRect (getLocalBounds(), borderWidth);

    g.setColour (juce::Colours::white);
    g.setFont (
        juce::Font (juce::FontOptions (titleFontSize)));

    g.drawText (
        "Oscillator / FM Modulation",
        getLocalBounds()
            .reduced (panelPadding)
            .removeFromTop (titleHeight),
        juce::Justification::centredLeft);
}

void OscComponent::resized()
{
    auto content = getLocalBounds().reduced (panelPadding);

    content.removeFromTop (titleHeight);

    waveTypeLabel.setBounds (
        content.removeFromTop (labelHeight));

    oscWaveSelector.setBounds (
        content.removeFromTop (comboHeight));

    content.removeFromTop (sectionGap);

    const auto controlWidth =
        (content.getWidth() - controlGap) / 2;

    auto frequencyArea = content.removeFromLeft (controlWidth);
    content.removeFromLeft (controlGap);

    fmFrequencyLabel.setBounds (
        frequencyArea.removeFromTop (labelHeight));

    fmFrequencySlider.setBounds (frequencyArea);

    fmDepthLabel.setBounds (
        content.removeFromTop (labelHeight));

    fmDepthSlider.setBounds (content);
}

void OscComponent::setSliderAndLabel (
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

// these two help us to paint over the exisiting fmDepth to visualize how our voice is changing things

void OscComponent::setEffectiveFmDepth(float depth)
{
    effectiveFmDepth = depth;
    hasEffectiveFmDepth = true;

    repaint(fmDepthSlider.getBounds());
}

void OscComponent::paintOverChildren(juce::Graphics& g)
{
    if (!hasEffectiveFmDepth)
        return;

    const auto layout =
        fmDepthSlider.getLookAndFeel().getSliderLayout(fmDepthSlider);

    const auto knobArea = layout.sliderBounds.toFloat();

    const float centreX =
        static_cast<float>(fmDepthSlider.getX())
        + knobArea.getCentreX();

    const float centreY =
        static_cast<float>(fmDepthSlider.getY())
        + knobArea.getCentreY();

    const float radius =
        juce::jmin(knobArea.getWidth(), knobArea.getHeight()) * 0.5f
        - knobDrawingPadding
        + markerOffset;

    if (radius <= markerRadius)
        return;

    const double limitedDepth = juce::jlimit(
        fmDepthSlider.getMinimum(),
        fmDepthSlider.getMaximum(),
        static_cast<double>(effectiveFmDepth));

    const float position = static_cast<float>(
        fmDepthSlider.valueToProportionOfLength(limitedDepth));

    const auto rotary = fmDepthSlider.getRotaryParameters();

    const float angle =
        rotary.startAngleRadians
        + position
            * (rotary.endAngleRadians - rotary.startAngleRadians);

    const float markerX = centreX + std::sin(angle) * radius;
    const float markerY = centreY - std::cos(angle) * radius;

    g.setColour(juce::Colour(0xffc084fc));

    g.fillEllipse(
        markerX - markerRadius,
        markerY - markerRadius,
        markerRadius * 2.0f,
        markerRadius * 2.0f);
}
