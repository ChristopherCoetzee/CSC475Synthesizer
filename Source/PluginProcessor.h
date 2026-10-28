/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <atomic>
#include <array>

//==============================================================================
/**
*/
class CSC475SynthesizerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    CSC475SynthesizerAudioProcessor();
    ~CSC475SynthesizerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts;
    std::atomic<float> inputRms { 0.0f};
    int readWaveformSamples (float* destination, int maxSamples);
private:
    //==============================================================================
    juce::Synthesiser synthesizer;
    
    static constexpr int waveFormBufferSize = 32768;
    
    std::array<float, waveFormBufferSize> waveformSamples {};
    
    juce::AbstractFifo waveformFifo { waveFormBufferSize };
    
    juce::AudioProcessorValueTreeState::ParameterLayout makeParameters();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CSC475SynthesizerAudioProcessor)
    
};
