#pragma once

#include <JuceHeader.h>

class WaveRadioButton : public juce::TextButton
{
public:
    void paintButton(juce::Graphics& g,
                     bool isMouseOverButton,
                     bool isButtonDown) override
    {
        juce::ignoreUnused(isButtonDown);

        const bool selected = getToggleState();
        const float centreY = getHeight() * 0.5f;

        const auto accent = juce::Colour(0xffc084fc);
        const auto text = juce::Colour(0xfff5e9d5);

        // Small outlined selection circle
        g.setColour(selected ? accent : text);
        g.drawEllipse(2.0f, centreY - 6.0f,
                      12.0f, 12.0f, 1.5f);

        // Filled centre when selected
        if (selected)
            g.fillEllipse(5.0f, centreY - 3.0f,
                          6.0f, 6.0f);

        // Waveform name next to the circle
        g.setColour(isMouseOverButton ? accent : text);
        g.setFont(juce::Font(juce::FontOptions(11.0f)));

        g.drawText(
            getButtonText(),
            20, 0,
            juce::jmax(0, getWidth() - 20),
            getHeight(),
            juce::Justification::centredLeft);
    }
};

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
    
    WaveRadioButton sineButton;
    WaveRadioButton sawButton;
    WaveRadioButton squareButton;

    void updateWaveButtons();


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
