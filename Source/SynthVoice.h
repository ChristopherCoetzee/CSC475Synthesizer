

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "AdsrData.h"


class SynthVoice : public juce::SynthesiserVoice
{
    public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                         int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    
    void update(const float attack, const float decay, const float sustain, const float release);
    
    private:
        AdsrData adsr;
        juce::AudioBuffer<float> synthesizerBuffer;
    
        juce::dsp::Oscillator<float> oscillator { [](float i) {return i / juce::MathConstants<float>::pi;}};
        juce::dsp::Gain<float> gain;
        bool isPrepared { false };
        
};
