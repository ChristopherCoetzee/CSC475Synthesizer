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
    oscillator.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
    modAdsr.noteOn();
};
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    modAdsr.noteOff();
    
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
    
    oscillator.prepareToPlay(specification);
    filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
    modAdsr.setSampleRate(sampleRate);
    gain.prepare(specification);
    gain.setGainLinear(0.05f);
    
    isPrepared = true;
}

void SynthVoice::updateAdsr(const float attack, const float decay, const float sustain, const float release)
{
    adsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                int startSample, int numSamples)
{
    jassert(isPrepared);

    if (!isVoiceActive())
        return;

    synthesizerBuffer.setSize(outputBuffer.getNumChannels(),
                              numSamples, false, false, true);
    synthesizerBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock { synthesizerBuffer };

    oscillator.getNextAudioBlock(audioBlock);

    adsr.applyEnvelopeToBuffer(synthesizerBuffer, 0, numSamples);

    filter.process(synthesizerBuffer);

    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample,
                             synthesizerBuffer, channel, 0, numSamples);
    }

    if (!adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::updateFilter (const int filterType, const float cutoff, const float resonance)
{
    float modulator = modAdsr.getNextSample();
    filter.updateParameters(filterType, cutoff,resonance, modulator);
}

void SynthVoice::updateModAdsr (const float attack, const float decay, const float sustain, const float release)
{
    modAdsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    
}
