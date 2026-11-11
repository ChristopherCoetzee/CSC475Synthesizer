#include "OscComponent.h"
#include <cmath>

namespace
{
    constexpr int panelPadding = 6;
    constexpr int titleHeight = 17;
    constexpr int labelHeight = 13;
    constexpr int comboHeight = 20;

    constexpr int sectionGap = 3;
    constexpr int controlGap = 6;

    constexpr int textBoxWidth = 72;
    constexpr int textBoxHeight = 16;
    constexpr int borderWidth = 1;

    constexpr float titleFontSize = 12.0f;
    constexpr float labelFontSize = 11.0f;

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
    oscWaveSelector.addItemList(
        juce::StringArray { "Sine", "Saw", "Square" },
        1);

    oscWaveSelector.setName ("Wave Type");
    addAndMakeVisible (oscWaveSelector);

    oscillatorWaveSelectorAttachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::ComboBoxAttachment>
            (apvts, selectorId, oscWaveSelector);
    
    
    // Keep the dropdown connected, but hide it
    oscWaveSelector.setVisible(false);

    sineButton.setButtonText("Sine");
    sawButton.setButtonText("Saw");
    squareButton.setButtonText("Square");

    addAndMakeVisible(sineButton);
    addAndMakeVisible(sawButton);
    addAndMakeVisible(squareButton);

    // Each button selects the matching parameter option
    sineButton.onClick = [this]()
    {
        oscWaveSelector.setSelectedId(1, juce::sendNotificationSync);
    };

    sawButton.onClick = [this]()
    {
        oscWaveSelector.setSelectedId(2, juce::sendNotificationSync);
    };

    squareButton.onClick = [this]()
    {
        oscWaveSelector.setSelectedId(3, juce::sendNotificationSync);
    };

    oscWaveSelector.onChange = [this]()
    {
        updateWaveButtons();
    };

    updateWaveButtons();


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

void OscComponent::updateWaveButtons()
{
    const int selected = oscWaveSelector.getSelectedId();

    sineButton.setToggleState(
        selected == 1, juce::dontSendNotification);

    sawButton.setToggleState(
        selected == 2, juce::dontSendNotification);

    squareButton.setToggleState(
        selected == 3, juce::dontSendNotification);
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
    auto content = getLocalBounds().reduced(panelPadding);

    // Space for panel title
    content.removeFromTop(titleHeight);

    // Hide old dropdown and label
    waveTypeLabel.setVisible(false);
    oscWaveSelector.setVisible(false);
    oscWaveSelector.setBounds(0, 0, 0, 0);

    // Left column: waveform radio buttons
    auto waveArea = content.removeFromLeft(100);

    content.removeFromLeft(controlGap);

    const int rowHeight = waveArea.getHeight() / 3;

    sineButton.setBounds(
        waveArea.removeFromTop(rowHeight));

    sawButton.setBounds(
        waveArea.removeFromTop(rowHeight));

    squareButton.setBounds(waveArea);

    // Right columns: FM knobs
    const int knobWidth =
        (content.getWidth() - controlGap) / 2;

    auto frequencyArea =
        content.removeFromLeft(knobWidth);

    content.removeFromLeft(controlGap);

    fmFrequencyLabel.setBounds(
        frequencyArea.removeFromTop(labelHeight));

    fmFrequencySlider.setBounds(frequencyArea);

    fmDepthLabel.setBounds(
        content.removeFromTop(labelHeight));

    fmDepthSlider.setBounds(content);
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
    if (!hasEffectiveFmDepth
        || fmDepthSlider.getWidth() <= 0
        || fmDepthSlider.getHeight() <= 0)
    {
        return;
    }

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
