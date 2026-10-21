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
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    
    mostRecentMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock (juce::dsp::AudioBlock<float>& block)
{
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            fmMod = fmOscillator.processSample(block.getSample(channel, s)) * fmDepth;
        }
    }
    
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OscData::setFMParameters (const float depth, const float freq)
{
    fmOscillator.setFrequency(freq);
    fmDepth = depth;
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(mostRecentMidiNote) + fmMod);
}
