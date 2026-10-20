/*
  ==============================================================================

    SynthesizerVoice.cpp
    Created: 4 Oct 2026 5:47:04pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
};
void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();
};
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    
    if (! allowTailOff || ! adsr.isActive())
    {
        clearCurrentNote();
    }
};
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
};

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec specification;
    specification.maximumBlockSize = samplesPerBlock;
    specification.sampleRate = sampleRate;
    specification.numChannels = outputChannels;
    
    oscillator.prepare(specification);
    
    
    gain.prepare(specification);
    
    gain.setGainLinear(0.32f);
    
    isPrepared = true;
}

void SynthVoice::update(const float attack, const float decay, const float sustain, const float release)
{
    adsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                int startSample, int numSamples)
{
    jassert(isPrepared); // if isPrepared is false it means that preparetoplay hasnt run and so we stop execution of the synth
    
    if(! isVoiceActive())
    {
        return;
    }
    
    synthesizerBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthesizerBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { synthesizerBuffer };
    oscillator.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    adsr.applyEnvelopeToBuffer(synthesizerBuffer,0, synthesizerBuffer.getNumSamples());
    
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthesizerBuffer, channel, 0, numSamples);
        
        if(! adsr.isActive()) {
            clearCurrentNote();
        }
    }
    
};

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    
}
