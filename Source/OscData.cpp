/*
  ==============================================================================

    OscData.cpp
    Created: 21 Oct 2026 1:28:28pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#include "OscData.h"

// we are gonna extend these so we have more sounds

void OscData::prepareToPlay (juce::dsp::ProcessSpec& spec)
{
    fmOscillator.prepare(spec);
    prepare(spec);
}

void OscData::setWaveType(const int choice)
{
    switch(choice) {
        case 0:
            initialise ([](float x) {return std::sin(x);});
            break;
        case 1:
            initialise ([](float x) {return x /juce::MathConstants<float>::pi;});
            break;
        case 2:
            initialise ([](float x) {return x < 0.0f ? -1.0f : 1.0f;});
            break;
        default:
            jassertfalse;
            break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    
    mostRecentMidiNote = midiNoteNumber;
    
    const float baseFrequency = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    
    setFrequency(baseFrequency, true);
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    const float baseFrequency = static_cast<float>(
        juce::MidiMessage::getMidiNoteInHertz(
            static_cast<int>(mostRecentMidiNote)));

    const int numSamples = static_cast<int>(block.getNumSamples());
    const int numChannels = static_cast<int>(block.getNumChannels());

    for (int sample = 0; sample < numSamples; ++sample)
    {
        fmMod = fmOscillator.processSample(0.0f) * fmDepth;

        const float currentFrequency =
            std::abs(baseFrequency + fmMod);

        setFrequency(currentFrequency, true);

        const float outputSample = processSample(0.0f);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            block.setSample(channel, sample, outputSample);
        }
    }
}

void OscData::setFMParameters (const float depth, const float freq)
{
    fmDepth = depth;
    fmOscillator.setFrequency(freq);
}
