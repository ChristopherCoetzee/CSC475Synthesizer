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
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", makeParameters())
#endif
{
    synthesizer.addSound(new SynthSound());

    for (int i = 0; i < 16; ++i)
    {
        synthesizer.addVoice(new SynthVoice());
    }

    synthesizer.setNoteStealingEnabled(false);
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
void CSC475SynthesizerAudioProcessor::prepareToPlay(
    double sampleRate, int samplesPerBlock)
{
    synthesizer.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synthesizer.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synthesizer.getVoice(i)))
        {
            voice->prepareToPlay(
                sampleRate,
                samplesPerBlock,
                getTotalNumOutputChannels());
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
    
    const auto inputChannels = layouts.getMainInputChannelSet();
    
    if (inputChannels != juce::AudioChannelSet::disabled()
     && inputChannels != juce::AudioChannelSet::mono()
     && inputChannels != juce::AudioChannelSet::stereo())
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
    
    float measuredRms = 0.0f;
    if (totalNumInputChannels > 0 && buffer.getNumSamples() > 0)
    {
        measuredRms = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    }
    
    inputRms.store(measuredRms);
    
    // we are writing the input samples for our waveform display into a buffer so that they can be read
    
    if (totalNumInputChannels > 0 && buffer.getNumSamples() > 0)
    {
        int start1 = 0;
        int size1 = 0;
        int start2 = 0;
        int size2 = 0;
        
        waveformFifo.prepareToWrite(buffer.getNumSamples(), start1, size1, start2, size2);
        
        const float* inputSamples = buffer.getReadPointer(0);
        
        if (size1 > 0 )
        {
            juce::FloatVectorOperations::copy (waveformSamples.data() + start1, inputSamples, size1);
        }
        
        if (size2 > 0)
        {
            juce::FloatVectorOperations::copy (
                waveformSamples.data() + start2,
                inputSamples + size1,
                size2);
        }
        
        waveformFifo.finishedWrite(size1 + size2);
    }
    
    for (auto i = 0; i < totalNumOutputChannels; ++i)
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
            
            auto& modAttack = *apvts.getRawParameterValue("MODATTACK");
            auto& modDecay = *apvts.getRawParameterValue("MODDECAY");
            auto& modSustain = *apvts.getRawParameterValue("MODSUSTAIN" );
            auto& modRelease = *apvts.getRawParameterValue("MODRELEASE");
            
            auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
            auto& filterFrequency = *apvts.getRawParameterValue("FILTERFREQUENCY");
            auto& filterResonance = *apvts.getRawParameterValue("FILTERRESONANCE");
            
            
            
            voice->getOscillator().setWaveType(oscWave);
            voice->getOscillator().setFMParameters(fmDepth, fmFrequency);
            voice->updateAdsr(attack, decay, sustain, release);
            voice->updateFilter(filterType, filterFrequency,filterResonance);
            voice->updateModAdsr(modAttack, modDecay,modSustain, modRelease);
        }
    }
    
    synthesizer.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples()); // renders the synths sounds
    
    // we filter here
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
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"FMFREQUENCY", 1}, "FM Frequency",
        juce::NormalisableRange<float>{0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"FMDEPTH", 1}, "FM Depth",
        juce::NormalisableRange<float>{0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
      
    

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
    
    parameters.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"OSC1WAVETYPE", 1}, "Oscillator 1 Wave Type",
        juce::StringArray{"Sin", "Saw", "Square"}, 0));
    
    // filter adsr
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"MODATTACK", 1}, "Mod Attack",
        juce::NormalisableRange<float>{0.1f, 1.0f}, 0.1f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"MODDECAY", 1}, "Mod Decay",
        juce::NormalisableRange<float>{0.1f, 1.0f}, 0.1f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"MODSUSTAIN", 1}, "Mod Sustain",
        juce::NormalisableRange<float>{0.1f, 1.0f}, 1.0f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"MODRELEASE", 1}, "Mod Release",
        juce::NormalisableRange<float>{0.1f, 3.0f}, 0.4f));
    
    
    parameters.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"FILTERTYPE", 1}, "Filter Type",
        juce::StringArray{"Low-Pass", "Band-Pass", "High-Pass"}, 0));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"FILTERFREQUENCY", 1}, "Filter Frequency",
        juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.6f}, 200.0f));
   
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"FILTERRESONANCE", 1}, "Filter Resonance",
        juce::NormalisableRange<float>{1.0f, 10.0f, 0.1f}, 1.0f));

    return {parameters.begin(), parameters.end()};}

int CSC475SynthesizerAudioProcessor::readWaveformSamples(float* destination, int maxSamples)
{
    if (destination == nullptr || maxSamples <= 0)
        return 0;
    
    int start1 = 0;
    int size1 = 0;
    int start2 = 0;
    int size2 = 0;
    
    waveformFifo.prepareToRead(maxSamples,start1,size1, start2,size2);
    
    if (size1 > 0)
    {
        juce::FloatVectorOperations::copy(destination, waveformSamples.data() + start1, size1);
    }
    
    if (size2 > 0)
    {
        juce::FloatVectorOperations::copy (
            destination + size1,
            waveformSamples.data() + start2,
            size2);
    }
    
    const int samplesRead = size1 + size2;
    
    waveformFifo.finishedRead(samplesRead);
    
    return samplesRead;
}
