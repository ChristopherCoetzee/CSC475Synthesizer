/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 18 Oct 2026 8:09:10pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::String name,juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId,juce::String sustainId,juce::String releaseId )
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    componentName = name;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    attackAttachment = std::make_unique<SliderAttachment>(apvts, attackId, attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, decayId, decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, sustainId, sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, releaseId, releaseSlider);
    
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,60,30);
    addAndMakeVisible(attackSlider);
    
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,60,30);
    addAndMakeVisible(decaySlider);
    
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,60,30);
    addAndMakeVisible(sustainSlider);
    
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,60,30);
    addAndMakeVisible(releaseSlider);

}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::blueviolet);
}

void AdsrComponent::resized()
{
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight =  bounds.getHeight();
    const auto sliderStartX = 0;
    const auto sliderStartY = 0;
    
    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding,  sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding,  sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding,  sliderStartY, sliderWidth, sliderHeight);

}
