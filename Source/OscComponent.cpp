/*
  ==============================================================================

    OscComponent.cpp
    Created: 21 Oct 2026 2:14:11pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFrequencyId, juce::String fmDepthId)
{
    juce::StringArray choices {"Sine", "Saw", "Square"};
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);
    
    oscillatorWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);
    
    setSliderAndLabel(fmFrequencySlider,fmFrequencyLabel, apvts, fmFrequencyId, fmFrequencyAttachment);
    setSliderAndLabel(fmDepthSlider,fmDepthLabel, apvts, fmDepthId, fmDepthAttachment);

}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void OscComponent::resized()
{
    oscWaveSelector.setBounds(0, 0, 90, 30);
    fmFrequencySlider.setBounds(0, 80, 100, 90);
    fmFrequencyLabel.setBounds(fmFrequencySlider.getX(), fmFrequencySlider.getY() -20, fmFrequencySlider.getWidth(), 20);
    fmDepthSlider.setBounds(fmFrequencySlider.getRight(), 80, 100, 90);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - 20, fmDepthSlider.getWidth(), 20);
}

void OscComponent:: setSliderAndLabel(juce::Slider& slider, juce::Label& label,juce::AudioProcessorValueTreeState& apvts,juce::String paramId,  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment )
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
