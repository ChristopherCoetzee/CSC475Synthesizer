/*
  ==============================================================================

    FilterComponent.cpp
    Created: 25 Oct 2026 3:49:18pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterFrequencyId, juce::String filterResonanceId)
{
    juce::StringArray choices {"Low-Pass", "Band-Pass", "High-Pass"};
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);
    
    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorId, filterTypeSelector);
    
    filterSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterSelectorLabel.setFont(juce::Font(juce::FontOptions{15.0f}));
    filterSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(filterSelectorLabel);
    
    setSliderAndLabel(filterFrequencySlider, filterFrequencyLabel, apvts, filterFrequencyId, filterFrequencyAttachment);
    setSliderAndLabel(filterResonanceSlider, filterResonanceLabel, apvts, filterResonanceId, filterResonanceAttachment);

}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void FilterComponent::resized()
{
    filterTypeSelector.setBounds(0, 0, 90, 30);
    filterFrequencySlider.setBounds(0, 80, 100, 90);
    filterFrequencyLabel.setBounds(filterFrequencySlider.getX(), filterFrequencySlider.getY() -20, filterFrequencySlider.getWidth(), 20);
    filterResonanceSlider.setBounds(filterFrequencySlider.getRight(), 80, 100, 90);
    filterResonanceLabel.setBounds(filterResonanceSlider.getX(), filterResonanceSlider.getY() - 20, filterResonanceSlider.getWidth(), 20);
    
    // need to add the labels for both selector comboboxes...

}
void FilterComponent:: setSliderAndLabel(juce::Slider& slider, juce::Label& label,juce::AudioProcessorValueTreeState& apvts,juce::String paramId,  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment )
{
    slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 30);
    
    addAndMakeVisible(slider);
    
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramId, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(juce::Font(juce::FontOptions(16.0f)));
    label.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(label);
}

