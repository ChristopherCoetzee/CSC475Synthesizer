/*
  ==============================================================================

    FilterData.h
    Created: 25 Oct 2026 2:52:28pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData
{
    public:
        void prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels);
        void process(juce::AudioBuffer<float>& buffer);
        void reset();
        void updateParameters(const int filterType, const float frequency, const float resonance);
    
    private:
        juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared { false };
    
};
