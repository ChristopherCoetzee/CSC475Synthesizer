/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
CSC475SynthesizerAudioProcessor::CSC475SynthesizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", makeParameters())
#endif
{
    synthesizer.addSound(new SynthSound());
    synthesizer.addVoice(new SynthVoice());
}

CSC475SynthesizerAudioProcessor::~CSC475SynthesizerAudioProcessor()
{
}

//==============================================================================
const juce::String CSC475SynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CSC475SynthesizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CSC475SynthesizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CSC475SynthesizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CSC475SynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CSC475SynthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CSC475SynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CSC475SynthesizerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CSC475SynthesizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void CSC475SynthesizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CSC475SynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synthesizer.setCurrentPlaybackSampleRate(sampleRate);
    
    // prepare all our voices
    
    for (int x = 0; x < synthesizer.getNumVoices(); x++)
        
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synthesizer.getVoice(x)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumInputChannels());
        }
    }
}

void CSC475SynthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CSC475SynthesizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CSC475SynthesizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int i = 0; i < synthesizer.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synthesizer.getVoice(i))) {
            
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN" );
            auto& release = *apvts.getRawParameterValue("RELEASE");
            
            auto& oscWave = *apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& fmDepth =  *apvts.getRawParameterValue("FMDEPTH");
            auto& fmFrequency =  *apvts.getRawParameterValue("FMFREQUENCY");
            
            
            
            voice->getOscillator().setWaveType(oscWave);
            voice->getOscillator().setFMParameters(fmDepth, fmFrequency);
            voice->update(attack, decay, sustain, release);
        }
    }
    
    synthesizer.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples()); // renders the synths sounds
    
};

//==============================================================================
bool CSC475SynthesizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CSC475SynthesizerAudioProcessor::createEditor()
{
    return new CSC475SynthesizerAudioProcessorEditor (*this);
}

//==============================================================================
void CSC475SynthesizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CSC475SynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CSC475SynthesizerAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout CSC475SynthesizerAudioProcessor::makeParameters()
{
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"OSC", 1}, "Oscillator",
        juce::StringArray{"Sine", "Saw", "Square"}, 0));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQUENCY", "FM Frequency", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH", "FM Depth", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
      
    

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"ATTACK", 1}, "attack",
        juce::NormalisableRange<float>{0.1f, 1.0f}, 0.1f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"DECAY", 1}, "decay",
        juce::NormalisableRange<float>{0.1f, 1.0f}, 0.1f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"SUSTAIN", 1}, "sustain",
        juce::NormalisableRange<float>{0.1f, 1.0f}, 1.0f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"RELEASE", 1}, "release",
        juce::NormalisableRange<float>{0.1f, 3.0f}, 0.4f));
    
    parameters.push_back(std::make_unique<juce::AudioParameterChoice> ("OSC1WAVETYPE", "Oscillator 1 Wave Type",juce::StringArray { "Sin", "Saw", "Square" }, 0));

    return {parameters.begin(), parameters.end()};}
