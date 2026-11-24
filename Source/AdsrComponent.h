
#pragma once
#include "KittyFont.h"
#include <JuceHeader.h>

// Custom cat-paw slider appearance
class CatPawSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void setPawColour(juce::Colour colour)
    {
        pawColour = colour;
    }

    void drawLinearSlider(
        juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& slider) override
    {
        if (style != juce::Slider::LinearVertical)
        {
            juce::LookAndFeel_V4::drawLinearSlider(
                                                   g, x, y, width, height,
                                                   sliderPos, minSliderPos, maxSliderPos,
                                                   style, slider);
            return;
        }
        
        juce::ignoreUnused(minSliderPos, maxSliderPos);
        
        const float cx = x + width * 0.5f;
        const float top = y + 9.0f;
        const float bottom = y + height - 9.0f;
        
        const float cy = juce::jlimit(
                                      top, bottom, sliderPos);
        
        // Light slider track
        g.setColour(pawColour.withAlpha(0.28f));
        g.drawLine(cx, top, cx, bottom, 6.0f);
        
        // Coloured section of the track
        g.setColour(pawColour);
        g.drawLine(cx, cy, cx, bottom, 4.0f);
        
        // Small retro tick marks
        g.setColour(
                    pawColour.darker(0.35f).withAlpha(0.45f));
        
        for (int i = 0; i <= 8; ++i)
        {
            const float tickY =
            top + (bottom - top) * i / 8.0f;
            
            g.drawLine(
                       cx - 12.0f, tickY,
                       cx - 9.0f, tickY, 1.0f);
            
            g.drawLine(
                       cx + 9.0f, tickY,
                       cx + 12.0f, tickY, 1.0f);
        }
        
        
        // Small shadow under the paw cap
        g.setColour(juce::Colours::black.withAlpha(0.15f));
        g.fillEllipse(cx - 9.0f, cy - 8.0f,
                      19.0f, 19.0f);
        
        // Smaller cream-coloured paw cap
        g.setColour(juce::Colour(0xfffff9ee));
        g.fillEllipse(cx - 9.5f, cy - 9.5f,
                      19.0f, 19.0f);
        
        g.setColour(pawColour.darker(0.25f));
        g.drawEllipse(cx - 9.5f, cy - 9.5f,
                      19.0f, 19.0f, 1.2f);
        
        // Four smaller toe pads
        g.setColour(pawColour);
        
        g.fillEllipse(cx - 7.5f, cy - 4.5f,
                      3.0f, 4.0f);
        
        g.fillEllipse(cx - 4.0f, cy - 7.0f,
                      3.5f, 4.0f);
        
        g.fillEllipse(cx + 0.5f, cy - 7.0f,
                      3.5f, 4.0f);
        
        g.fillEllipse(cx + 4.5f, cy - 4.5f,
                      3.0f, 4.0f);
        
        // Main paw pad
        g.fillEllipse(cx - 5.5f, cy,
                      11.0f, 7.0f);
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

        // Numeric value
        
        // Numeric value
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
        return KittyFont::make(13.0f);
    }


private:
    juce::Colour pawColour {
        juce::Colour(0xff6faee8)
    };
};


// ADSR component
class AdsrComponent : public juce::Component
{
public:
    AdsrComponent(
        juce::String name,
        juce::AudioProcessorValueTreeState& apvts,
        juce::String attackId,
        juce::String decayId,
        juce::String sustainId,
        juce::String releaseId);

    ~AdsrComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Custom paw appearance, shared by all four sliders
    CatPawSliderLookAndFeel pawSliderLookAndFeel;

    // Sliders
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    // Labels
    juce::Label attackLabel { "Attack", "Attack" };
    juce::Label decayLabel { "Decay", "Decay" };
    juce::Label sustainLabel { "Sustain", "Sustain" };
    juce::Label releaseLabel { "Release", "Release" };

    juce::String componentName;

    // Parameter attachments
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        AdsrComponent)
};

