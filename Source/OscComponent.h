#pragma once
#include "KittyFont.h"
#include <JuceHeader.h>




class WaveRadioButton : public juce::TextButton
{
public:
    void setAccentColour(juce::Colour colour)
    {
        accentColour = colour;
        repaint();
    }

    void paintButton(juce::Graphics& g,
                     bool isMouseOverButton,
                     bool isButtonDown) override
    {
        const bool selected = getToggleState();

        // Small circular button
        const float diameter = 18.0f;
        const float cx = 16.0f;
        const float cy = getHeight() * 0.5f
                       + (isButtonDown ? 1.0f : 0.0f);

        auto circle = juce::Rectangle<float>(
            cx - diameter * 0.5f,
            cy - diameter * 0.5f,
            diameter,
            diameter);

        // Light effect only when selected
        if (selected)
        {
            // Soft outer glow
            g.setColour(accentColour.withAlpha(0.18f));
            g.fillEllipse(circle.expanded(5.0f));

            // Illuminated ring
            g.setColour(accentColour.withAlpha(0.55f));
            g.drawEllipse(circle.expanded(2.5f), 2.0f);
        }

        // Subtle shadow underneath
        g.setColour(
            juce::Colours::black.withAlpha(0.18f));

        g.fillEllipse(circle.translated(0.0f, 1.5f));

        // Darker outer rim
        g.setColour(accentColour.darker(0.45f));
        g.fillEllipse(circle);

        // Matte coloured button surface
        auto inner = circle.reduced(1.5f);

        if (selected)
        {
            // Brighter when activated
            g.setColour(accentColour.brighter(0.40f));
        }
        else if (isMouseOverButton)
        {
            // Slight change on hover
            g.setColour(accentColour.brighter(0.10f));
        }
        else
        {
            // Normal coloured button
            g.setColour(accentColour.darker(0.18f));
        }

        g.fillEllipse(inner);

        // No shiny highlight or reflection

        // Small label underneath the button
        g.setColour(accentColour.darker(0.65f));

        g.setFont(KittyFont::make(13.0));

    
        // Label directly to the right of the button
        g.drawFittedText(
            getButtonText(),
            33,
            0,
            juce::jmax(0, getWidth() - 35),
            getHeight(),
            juce::Justification::centredLeft,
            1);
    }

private:
    juce::Colour accentColour {
        juce::Colour(0xffde6b6b)
    };
};


class RetroMetalKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // false = oscillator red, true = filter orange
    explicit RetroMetalKnobLookAndFeel(bool orangeTheme = false)
        : orange(orangeTheme) {}

    void drawRotarySlider(
        juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider) override
    {
        juce::ignoreUnused(slider);

        // Section-specific metallic colours
        const auto bodyColour = orange
            ? juce::Colour(0xff82451f)
            : juce::Colour(0xff71343c);

        const auto edgeColour = orange
            ? juce::Colour(0xfff0a35a)
            : juce::Colour(0xffde6b6b);

        const auto ridgeColour = orange
            ? juce::Colour(0xffffd1a3)
            : juce::Colour(0xffe79c9e);

        const auto capLight = orange
            ? juce::Colour(0xfffff0db)
            : juce::Colour(0xffffe2d9);

        const auto capDark = orange
            ? juce::Colour(0xffc98445)
            : juce::Colour(0xffc66b72);

        const auto capOutline = orange
            ? juce::Colour(0xffa96831)
            : juce::Colour(0xffa54147);

        // Same small knob size for every section
        const float diameter = juce::jmin(
            56.0f,
            static_cast<float>(juce::jmin(width, height)) - 4.0f);

        if (diameter <= 0.0f)
            return;

        const float cx = x + width * 0.5f;
        const float cy = y + height * 0.5f;
        const float radius = diameter * 0.5f;

        auto body = juce::Rectangle<float>(
            cx - radius, cy - radius,
            diameter, diameter);

        // Shadow
        g.setColour(juce::Colours::black.withAlpha(0.18f));
        g.fillEllipse(body.translated(1.0f, 2.0f));

        // Coloured outer body
        g.setColour(bodyColour);
        g.fillEllipse(body);

        g.setColour(edgeColour);
        g.drawEllipse(body.reduced(1.0f), 2.0f);

        // Ridges around outer grip
        for (int i = 0; i < 24; ++i)
        {
            const float a =
                juce::MathConstants<float>::twoPi
                * static_cast<float>(i) / 24.0f;

            const float inner = radius * 0.79f;
            const float outer = radius * 0.94f;

            g.setColour(ridgeColour);

            g.drawLine(
                cx + std::sin(a) * inner,
                cy - std::cos(a) * inner,
                cx + std::sin(a) * outer,
                cy - std::cos(a) * outer,
                1.4f);
        }

        // Metallic centre cap
        const float capRadius = radius * 0.62f;

        auto cap = juce::Rectangle<float>(
            cx - capRadius, cy - capRadius,
            capRadius * 2.0f, capRadius * 2.0f);

        g.setColour(bodyColour);
        g.fillEllipse(cap.expanded(1.5f));

        g.setGradientFill(juce::ColourGradient(
            capLight,
            cx - capRadius, cy - capRadius,
            capDark,
            cx + capRadius, cy + capRadius,
            false));

        g.fillEllipse(cap);

        g.setColour(capOutline);
        g.drawEllipse(cap, 1.0f);

        // Rotating value indicator
        const float angle =
            rotaryStartAngle
            + sliderPosProportional
              * (rotaryEndAngle - rotaryStartAngle);

        const float innerPointer = capRadius * 0.28f;
        const float outerPointer = capRadius * 0.85f;

        g.setColour(bodyColour);

        g.drawLine(
            cx + std::sin(angle) * innerPointer,
            cy - std::cos(angle) * innerPointer,
            cx + std::sin(angle) * outerPointer,
            cy - std::cos(angle) * outerPointer,
            2.5f);
    }

    void drawLabel(juce::Graphics& g,
                   juce::Label& label) override
    {
        auto bounds = label.getLocalBounds()
                           .toFloat().reduced(1.0f, 1.0f);

        if (bounds.isEmpty())
            return;

        const auto accent =
            label.findColour(juce::Label::textColourId);

        // Shadow underneath the box
        g.setColour(juce::Colours::black.withAlpha(0.16f));
        g.fillRoundedRectangle(
            bounds.translated(0.0f, 1.0f), 3.0f);

        // Cream gradient creates a raised surface
        g.setGradientFill(juce::ColourGradient(
            juce::Colour(0xfffffff8),
            bounds.getX(), bounds.getY(),
            juce::Colour(0xffe9ded0),
            bounds.getX(), bounds.getBottom(),
            false));

        g.fillRoundedRectangle(bounds, 3.0f);

        // Coloured border
        g.setColour(accent.withAlpha(0.65f));
        g.drawRoundedRectangle(bounds, 3.0f, 1.0f);

        // Light reflection along the top
        g.setColour(juce::Colours::white.withAlpha(0.8f));
        g.drawLine(bounds.getX() + 3.0f,
                   bounds.getY() + 2.0f,
                   bounds.getRight() - 3.0f,
                   bounds.getY() + 2.0f, 1.0f);

        if (!label.isBeingEdited())
        {
            g.setColour(accent);
            g.setFont(KittyFont::make(14.0f));

            g.drawFittedText(
                label.getText(),
                label.getLocalBounds().reduced(3, 1),
                label.getJustificationType(),
                1);
        }
    }

    // Font for editable numeric value boxes
    juce::Font getLabelFont(juce::Label&) override
    {
        return KittyFont::make(14.0f);
    }

private:
    bool orange;
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


    // Retro appearance applied only to FM Frequency
    RetroMetalKnobLookAndFeel retroKnobLookAndFeel;
    
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
