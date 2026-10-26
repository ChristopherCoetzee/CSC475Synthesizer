/*
  ==============================================================================

    FilterData.cpp
    Created: 25 Oct 2026 2:52:28pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#include "FilterData.h"


void FilterData::prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels)
{
    filter.reset();
    juce::dsp::ProcessSpec specification;
    specification.maximumBlockSize = samplesPerBlock;
    specification.sampleRate = sampleRate;
    specification.numChannels = numChannels;
    
    filter.prepare(specification);
    isPrepared = true;
}
void FilterData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);
    
    juce::dsp::AudioBlock<float> audioBlock { buffer };
    filter.process(juce::dsp::ProcessContextReplacing<float> { audioBlock }); // replaces the content in the buffer with filtered audio
}
void FilterData::reset()
{
    filter.reset();
}
void FilterData::updateParameters(const int filterType, const float frequency, const float resonance)
{
    switch(filterType)
    {
        case 0:
            filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            break;
        case 1:
            filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        case 2:
            filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    }
    
    filter.setCutoffFrequency(frequency);
    filter.setResonance(resonance);
}
