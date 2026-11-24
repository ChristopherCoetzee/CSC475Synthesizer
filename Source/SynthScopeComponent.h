//
//  SynthScopeComponent.h
//  CSC475Synthesizer
//
//  Created by Isabella Nichol on 2026-11-24.
//


#pragma once

#include <JuceHeader.h>
#include <array>
#include <cmath>

class SynthScopeComponent : public juce::Component
{
public:
    void setColours(juce::Colour background,
                    juce::Colour waveform)
    {
        backgroundColour = background;
        waveformColour = waveform;
        
    }

    // Receive actual audio samples from the synth
    void pushSamples(const float* samples, int numSamples)
    {
        if (samples == nullptr || numSamples <= 0)
            return;

        for (int i = 0; i < numSamples; ++i)
        {
            history[writePosition] = samples[i];

            writePosition = (writePosition + 1) % historySize;
            storedSamples = juce::jmin(storedSamples + 1,
                                       historySize);
        }

        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(backgroundColour);

        if (getWidth() < 2 || getHeight() < 2 ||
            storedSamples < visibleSamples + triggerSearch + 1)
            return;

        // Read a sample from the circular history buffer
        auto sampleAt = [this](int index) -> float
        {
            return history[(index + historySize) % historySize];
        };

        // Start slightly behind the latest audio
        int start = (writePosition + historySize
                     - visibleSamples - triggerSearch)
                    % historySize;

        // Find a rising zero crossing to stabilize the shape
        for (int i = 1; i <= triggerSearch; ++i)
        {
            if (sampleAt(start + i - 1) <= 0.0f &&
                sampleAt(start + i) > 0.0f)
            {
                start = (start + i) % historySize;
                break;
            }
        }

        // Find the peak so the waveform uses the available height
        float peak = 0.0f;

        for (int i = 0; i < visibleSamples; ++i)
        {
            peak = juce::jmax(
                peak,
                std::abs(sampleAt(start + i)));
        }

        // Don't magnify silence or background numerical noise
        if (peak < 0.005f)
            return;

        const float centreY = getHeight() * 0.5f;
        const float amplitude = getHeight() * 0.42f;
        const float scale = amplitude / peak;

        juce::Path wavePath;

        // Draw a continuous line from individual samples
        for (int i = 0; i < visibleSamples; ++i)
        {
            const float x =
                static_cast<float>(i)
                / static_cast<float>(visibleSamples - 1)
                * static_cast<float>(getWidth() - 1);

            const float y =
                centreY - sampleAt(start + i) * scale;

            if (i == 0)
                wavePath.startNewSubPath(x, y);
            else
                wavePath.lineTo(x, y);
        }

        g.setColour(waveformColour);
        g.strokePath(wavePath, juce::PathStrokeType(2.0f));
    }

private:
    static constexpr int historySize = 4096;

    // Number of raw samples visible across the screen
    static constexpr int visibleSamples = 512;

    static constexpr int triggerSearch = 512;

    std::array<float, historySize> history {};

    int writePosition = 0;
    int storedSamples = 0;

    juce::Colour backgroundColour { 0xfffff06a };
    juce::Colour waveformColour { 0xffc040e0 };
};
