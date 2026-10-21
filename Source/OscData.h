/*
  ==============================================================================

    OscData.h
    Created: 21 Oct 2026 1:28:28pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
    public:
    void prepareToPlay (juce::dsp::ProcessSpec& spec);
    void setWaveType(const int choice);
    void setWaveFrequency(const int midiNoteNumber);
    void getNextAudioBlock (juce::dsp::AudioBlock<float>& block);
    void setFMParameters (const float depth, const float freq);
    
    private:
    juce::dsp::Oscillator<float> fmOscillator { [] (float x) {return std::sin(x);}};
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    float mostRecentMidiNote { 0 };
};
