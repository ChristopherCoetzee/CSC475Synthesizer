//
//  KittyFont.h
//  CSC475Synthesizer
//
//  Created by Isabella Nichol on 2026-11-23.
//

#pragma once

#include <JuceHeader.h>
#include <BinaryData.h>

namespace KittyFont
{
    inline juce::Typeface::Ptr getTypeface()
    {
        static auto typeface =
            juce::Typeface::createSystemTypefaceFor(
                BinaryData::KittyRegular_ttf,
                BinaryData::KittyRegular_ttfSize);

        return typeface;
    }

    inline juce::Font make(float size)
    {
        return juce::Font(
            juce::FontOptions { getTypeface() }
                .withHeight(size));
    }
}
