#include "OscComponent.h"
#include "KittyFont.h"
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
    constexpr int textBoxHeight = 20;
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
        juce::Colour(0xff604d60));

    waveTypeLabel.setFont(KittyFont::make(15.0f));

    waveTypeLabel.setJustificationType (
        juce::Justification::centredLeft);

    addAndMakeVisible (waveTypeLabel);

    setSliderAndLabel (
        fmFrequencySlider,
        fmFrequencyLabel,
        apvts,
        frequencyId,
        fmFrequencyAttachment);
    
    fmFrequencySlider.getProperties().set(
        "bvs_knobStyle",
        static_cast<int>(bvs::KnobStyle::Pro));

    fmFrequencySlider.setLookAndFeel(&retroKnobLookAndFeel);
    fmDepthSlider.setLookAndFeel(&retroKnobLookAndFeel);

    // Restore the value box
    fmFrequencySlider.setTextBoxStyle(
        juce::Slider::TextBoxBelow,
        false,
        72,
        16);

    // Keep the red oscillator colour and readable numbers
    fmFrequencySlider.setColour(
        juce::Slider::rotarySliderFillColourId,
        juce::Colour(0xffde6b6b));

    fmFrequencySlider.setColour(
        juce::Slider::thumbColourId,
        juce::Colour(0xffa54147));

    fmFrequencySlider.setColour(
        juce::Slider::textBoxTextColourId,
        juce::Colour(0xffa54147));

    fmFrequencySlider.setColour(
        juce::Slider::textBoxBackgroundColourId,
        juce::Colour(0xfffffdf6));

    setSliderAndLabel (
        fmDepthSlider,
        fmDepthLabel,
        apvts,
        amountId,
        fmDepthAttachment);
    // Apply the metallic retro appearance to FM Frequency only
    fmFrequencySlider.setLookAndFeel(&retroKnobLookAndFeel);

    // Keep its numeric value underneath
    fmFrequencySlider.setTextBoxStyle(
        juce::Slider::TextBoxBelow,
        false,
        65,
        16);
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

OscComponent::~OscComponent()
{
    fmFrequencySlider.setLookAndFeel(nullptr);
    fmDepthSlider.setLookAndFeel(nullptr);
}

void OscComponent::paint (juce::Graphics& g)
{
    // Warm white paper background
    g.fillAll(juce::Colour(0xfffffdf6));

    // Red oscillator border
    g.setColour(juce::Colour(0xffde6b6b));
    g.drawRect(getLocalBounds(), 2);

    // Soft dark text
    g.setColour(juce::Colour(0xffb24e4e));
    g.setFont(KittyFont::make(12.0f));

    g.drawText (
        "OSCILLATOR / FM MODULATION",
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
    
    // Red oscillator knobs and readable numbers
    const auto red = juce::Colour(0xffde6b6b);
    const auto darkRed = juce::Colour(0xffa54147);

    slider.setColour(
        juce::Slider::rotarySliderFillColourId, red);

    slider.setColour(
        juce::Slider::rotarySliderOutlineColourId,
        red.withAlpha(0.40f));

    slider.setColour(
        juce::Slider::thumbColourId, darkRed);

    slider.setColour(
        juce::Slider::textBoxTextColourId, darkRed);

    slider.setColour(
        juce::Slider::textBoxBackgroundColourId,
        juce::Colour(0xfffffdf6));

    slider.setColour(
        juce::Slider::textBoxOutlineColourId,
        darkRed.withAlpha(0.65f));

    addAndMakeVisible (slider);

    attachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::SliderAttachment>
            (apvts, paramId, slider);

    label.setColour (
        juce::Label::textColourId,
        juce::Colour(0xffa54147));

    label.setFont(KittyFont::make(15.0f));

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

    const float knobDiameter = juce::jmin(
        56.0f,
        juce::jmin(knobArea.getWidth(), knobArea.getHeight()) - 4.0f);

    const float radius = knobDiameter * 0.5f + 4.0f;

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

    g.setColour(juce::Colour(0xffde6b6b));

    g.fillEllipse(
        markerX - markerRadius,
        markerY - markerRadius,
        markerRadius * 2.0f,
        markerRadius * 2.0f);
}
