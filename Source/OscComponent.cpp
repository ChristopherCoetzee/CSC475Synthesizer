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
    
    fmFrequencySlider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    fmFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 30);
    
    addAndMakeVisible(fmFrequencySlider);
    
    fmFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, fmFrequencyId, fmFrequencySlider);
    
    fmFrequencyLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fmFrequencyLabel.setFont(juce::Font(juce::FontOptions(16.0f)));
    fmFrequencyLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(fmFrequencyLabel);
    

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
}
