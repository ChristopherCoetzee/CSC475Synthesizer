/*
  ==============================================================================

    FilterComponent.h
    Created: 25 Oct 2026 3:49:18pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterFrequencyId, juce::String filterResonanceId);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;
    
    juce::Slider filterFrequencySlider;
    juce::Slider filterResonanceSlider;
    juce::ComboBox filterTypeSelector { "Filter Type" };
    void setSliderAndLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts,juce::String paramId,  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment);
    
   
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterFrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    juce::Label filterSelectorLabel {"Filter Type", "Filter Type"};
    juce::Label filterFrequencyLabel {"Filter Frequency", "Filter Frequency"};
    juce::Label filterResonanceLabel {"Filter Resonance", "Filter Resonance"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
