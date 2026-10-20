/*
  ==============================================================================

    SourceCode.h
    Created: 18 Oct 2026 8:10:36pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
    public:
        void updateADSR(const float attack, const float decay, const float sustain, const float release);
        
    
    private:
        juce::ADSR::Parameters adsrParameters;
};
