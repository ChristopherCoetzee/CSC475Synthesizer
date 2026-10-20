/*
  ==============================================================================

    SourceCode.cpp
    Created: 18 Oct 2026 8:10:36pm
    Author:  Christopher Luke Coetzee

  ==============================================================================
*/

#include "AdsrData.h"

void AdsrData::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    adsrParameters.attack = attack;
    adsrParameters.decay = decay;
    adsrParameters.sustain = sustain;
    adsrParameters.release = release;
    
    setParameters(adsrParameters);
}
