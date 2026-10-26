/*
  ==============================================================================

    OscComponent.h
    Created: 21 Oct 2026 2:14:11pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFrequencyId, juce::String fmDepthId);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscillatorWaveSelectorAttachment;
    juce::Slider fmFrequencySlider;
    juce::Slider fmDepthSlider;
    
    juce::Label waveTypeLabel {"Wave Type", "Wave Type"}; // finish this later
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmFrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmDepthAttachment;
    juce::Label fmFrequencyLabel {"FM Frequency", "FM Frequency"};
    juce::Label fmDepthLabel {"FM Depth", "FM Depth"};
    
    
     
    void setSliderAndLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts,juce::String paramId,  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment);
    
    
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
