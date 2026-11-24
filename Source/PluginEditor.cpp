#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "KittyFont.h"
#include "SynthScopeComponent.h"

namespace
{
namespace Layout
{
constexpr int editorWidth = 1100;
constexpr int editorHeight = 800;


constexpr int bannerHeight = 340;

constexpr int margin = 8;
constexpr int gap = 6;

constexpr int controlsWidth = 650;
constexpr int topRowHeight = 145;

constexpr int inputControlsHeight = 52;
constexpr int midiHeight = 64;

constexpr int panelPadding = 6;
constexpr int titleHeight = 25;
constexpr int borderWidth = 1;

constexpr float titleFontSize = 20.0f;
constexpr float noteFontSize = 16.0f;

constexpr int meterRefreshHz = 20;
constexpr int rmsDecimalPlaces = 5;

// these are for the input voices waveform

constexpr int waveFormHistorySize = 512;
constexpr int waveformSamplesPerPoint = 128;
constexpr int waveformMaxReadsPerTick = 8;

constexpr int rmsRowHeight = 18;
constexpr int centroidRowHeight = 18;
constexpr int flatnessRowHeight = 18;

namespace Colours
{
const auto paper        = juce::Colour(0xfffffdf6); // white paper
const auto textDark     = juce::Colour(0xff5f4a5f); // darker purple-grey for general text

const auto oscRed       = juce::Colour(0xffde6b6b);
const auto filterOrange = juce::Colour(0xfff0a35a);
const auto synthYellow  = juce::Colour(0xffe7cf57);
const auto ampBlue      = juce::Colour(0xff6faee8);
const auto modGreen     = juce::Colour(0xff7fc97f);
const auto inputPink    = juce::Colour(0xffe694bd);
const auto midiPurple   = juce::Colour(0xffaf8ee6);

const auto oscText      = juce::Colour(0xffb24e4e);
const auto filterText   = juce::Colour(0xffb56f33);
const auto synthText    = juce::Colour(0xff9f8b1d);
const auto ampText      = juce::Colour(0xff4e85b8);
const auto modText      = juce::Colour(0xff4f9550);
const auto inputText    = juce::Colour(0xffb85b8e);
const auto midiText     = juce::Colour(0xff7a5db3);
}





}


void drawReservedPanel(
                       juce::Graphics& g,
                       juce::Rectangle<int> bounds,
                       const juce::String& title,
                       const juce::String& note)
    {
        // Each section has its own border and title colour
        auto borderColour = Layout::Colours::midiPurple;
        auto titleColour = Layout::Colours::midiText;

        if (title.startsWith("Synth Output"))
        {
            borderColour = Layout::Colours::synthYellow;
            titleColour = juce::Colour(0xffb84be8);
        }
        
        else if (title.startsWith("Live voice")
                 || title.startsWith("Sound input"))
        {
            borderColour = Layout::Colours::inputPink;
            titleColour = juce::Colour(0xfff88b18);
        }
            
        // Paper-coloured panel background
        g.setColour(juce::Colour(0xfffffdf6));
        g.fillRect(bounds);
        
        // Coloured border
        g.setColour(borderColour);
        g.drawRect(bounds, 2);
        
        auto content = bounds.reduced(Layout::panelPadding);
        
        // Playful handwritten headings
        const bool isLivePanel =
        title.startsWithIgnoreCase("Live voice");

    // Orange heading for live-input panel only
    // Use each panel's own title colour
        g.setColour(titleColour);

        g.setFont(
                  KittyFont::make(
                                  isLivePanel ? 17.0f :    Layout::titleFontSize));
        
        g.drawFittedText(
            title.toUpperCase(),
            content.removeFromTop(Layout::titleHeight),
            juce::Justification::centredLeft,
            1);
        
        // Smaller description text
        g.setColour(Layout::Colours::textDark);

        g.setFont(KittyFont::make(Layout::noteFontSize));
        
        g.drawFittedText(
                         note,
                         content,
                         juce::Justification::centredLeft,
                         2);
        }
}


CSC475SynthesizerAudioProcessorEditor::
CSC475SynthesizerAudioProcessorEditor (
    CSC475SynthesizerAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , audioProcessor (p)
    , osc (
        audioProcessor.apvts,
        "OSC1WAVETYPE",
        "FMFREQUENCY",
        "FMDEPTH")
    , adsr (
        "Amplitude ADSR",
        audioProcessor.apvts,
        "ATTACK",
        "DECAY",
        "SUSTAIN",
        "RELEASE")
    , modAdsr (
        "Modulation ADSR",
        audioProcessor.apvts,
        "MODATTACK",
        "MODDECAY",
        "MODSUSTAIN",
        "MODRELEASE")
    , filter (
        audioProcessor.apvts,
        "FILTERTYPE",
        "FILTERFREQUENCY",
        "FILTERRESONANCE")
{
    const auto background = Layout::Colours::paper;
    const auto foreground = Layout::Colours::textDark;
    const auto outline    = juce::Colour(0xffa08e9a);
    const auto accent     = Layout::Colours::inputPink;

    blackLookAndFeel.setColour (
        juce::ResizableWindow::backgroundColourId, background);

    blackLookAndFeel.setColour (
        juce::Label::textColourId, foreground);

    // Sliders and their value boxes.
    blackLookAndFeel.setColour (
        juce::Slider::backgroundColourId, outline);

    blackLookAndFeel.setColour (
        juce::Slider::thumbColourId, foreground);

    blackLookAndFeel.setColour (
        juce::Slider::trackColourId, accent);

    blackLookAndFeel.setColour (
        juce::Slider::rotarySliderFillColourId, accent);

    blackLookAndFeel.setColour (
        juce::Slider::rotarySliderOutlineColourId, outline);

    blackLookAndFeel.setColour (
        juce::Slider::textBoxTextColourId, foreground);

    blackLookAndFeel.setColour (
        juce::Slider::textBoxBackgroundColourId, background);

    blackLookAndFeel.setColour (
        juce::Slider::textBoxOutlineColourId, outline);

    blackLookAndFeel.setColour (
        juce::Slider::textBoxHighlightColourId, outline);

    // Combo boxes.
    blackLookAndFeel.setColour (
        juce::ComboBox::backgroundColourId, background);

    blackLookAndFeel.setColour (
        juce::ComboBox::textColourId, foreground);

    blackLookAndFeel.setColour (
        juce::ComboBox::outlineColourId, outline);

    blackLookAndFeel.setColour (
        juce::ComboBox::focusedOutlineColourId, accent);

    blackLookAndFeel.setColour (
        juce::ComboBox::arrowColourId, foreground);

    blackLookAndFeel.setColour (
        juce::ComboBox::buttonColourId, background);

    // Dropdown menus.
    blackLookAndFeel.setColour (
        juce::PopupMenu::backgroundColourId, background);

    blackLookAndFeel.setColour (
        juce::PopupMenu::textColourId, foreground);

    blackLookAndFeel.setColour (
        juce::PopupMenu::highlightedBackgroundColourId, outline);

    blackLookAndFeel.setColour (
        juce::PopupMenu::highlightedTextColourId, foreground);

    setLookAndFeel (&blackLookAndFeel);
   


    addAndMakeVisible(osc);
    addAndMakeVisible(filter);
    addAndMakeVisible(adsr);
    addAndMakeVisible(modAdsr);
    addAndMakeVisible(voiceWaveform);
    

    addAndMakeVisible(synthWaveform);


    voiceWaveform.setSamplesPerBlock(Layout::waveformSamplesPerPoint);
    voiceWaveform.setBufferSize(Layout::waveFormHistorySize);
    // Microphone input: red waveform
    voiceWaveform.setColours(
        juce::Colour(0xffffe3ed),
        juce::Colour(0xfff88b18));

    // Synth output: green waveform
    synthWaveform.setColours(
        juce::Colour(0xfffff06a),
        juce::Colour(0xffc040e0));

    voiceWaveform.setRepaintRate(Layout::meterRefreshHz);

    catImage = juce::ImageCache::getFromMemory(
        BinaryData::cats_png,
        BinaryData::cats_pngSize);
    
    setSize (Layout::editorWidth, Layout::editorHeight);

    startTimerHz(Layout::meterRefreshHz);
}

CSC475SynthesizerAudioProcessorEditor::
~CSC475SynthesizerAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
}
void CSC475SynthesizerAudioProcessorEditor::paint(
    juce::Graphics& g)
{
    
    // Background behind the UI
    g.fillAll(juce::Colour(0xffc8faf3)); // same sky blue as the cat image

    // Draw the cat banner directly with no inner white gap or border
    if (catImage.isValid())
    {
        juce::Path bannerClip;
        bannerClip.addRoundedRectangle(catBannerBounds.toFloat(), 14.0f);

        g.saveState();
        g.reduceClipRegion(bannerClip);

        g.drawImage(
            catImage,
            catBannerBounds.toFloat(),
            juce::RectanglePlacement::fillDestination);

        g.restoreState();
    }


    //g.setColour(Layout::Colours::synthYellow);
   // g.drawRoundedRectangle(
        //catBannerBounds.toFloat(), 14.0f, 1.0f);


    drawReservedPanel (
        g,
        audioVisualBounds,
        "Synth Output Waveform",
        "");

    drawReservedPanel (
        g,
        voiceVisualBounds,
        "Live voice input waveform",
        "");

    auto rmsArea =
        voiceVisualBounds.reduced (Layout::panelPadding);

    rmsArea.removeFromTop (Layout::titleHeight);

    const float currentInputRms = audioProcessor.inputRms.load();

    // Darker orange from the cat's colour family
    g.setColour(juce::Colour(0xfff88b18));

    // Smaller handwritten font
    g.setFont(KittyFont::make(14.0f));

    g.drawText (
        juce::String ("Input RMS: ")
            + juce::String (currentInputRms, Layout::rmsDecimalPlaces),
        rmsArea.removeFromTop (Layout::rmsRowHeight),
        juce::Justification::centredLeft);

    const float currentCentroidHz =
        audioProcessor.inputCentroidHz.load();

    const juce::String centroidText =
        currentCentroidHz > 0.0f
            ? juce::String("Voice centroid: ")
                + juce::String(currentCentroidHz, 0)
                + " Hz"
            : juce::String("Voice centroid: N/A");

    g.drawText(
        centroidText,
        rmsArea.removeFromTop(Layout::centroidRowHeight),
        juce::Justification::centredLeft);

    g.drawText(
        juce::String("Spectral flatness: ")
            + juce::String(audioProcessor.inputSpectralFlatness.load(), 3),
        rmsArea.removeFromTop(Layout::flatnessRowHeight),
        juce::Justification::centredLeft);

    drawReservedPanel (
        g,
        inputControlsBounds,
        "Sound input controls",
        "Reserved for input source selection and associated buttons.");

    drawReservedPanel (
        g,
        midiBounds,
        "Software MIDI",
        "Reserved for an on-screen MIDI keyboard and controls.");
}


void CSC475SynthesizerAudioProcessorEditor::resized()
{
    auto content = getLocalBounds().reduced(Layout::margin);

    // 1. Large cat illustration at the top
    catBannerBounds =
        content.removeFromTop(Layout::bannerHeight);

    content.removeFromTop(Layout::gap);

    // 2. MIDI keyboard at the very bottom
    midiBounds =
        content.removeFromBottom(Layout::midiHeight);

    content.removeFromBottom(Layout::gap);

    // 3. Sound input controls above the keyboard
    inputControlsBounds =
        content.removeFromBottom(Layout::inputControlsHeight);

    content.removeFromBottom(Layout::gap);

    // 4. Synth controls on the left
    auto controls =
        content.removeFromLeft(Layout::controlsWidth);

    content.removeFromLeft(Layout::gap);

    // 5. Visualizations on the right
    const int visualHeight =
        (content.getHeight() - Layout::gap) / 2;

    audioVisualBounds =
        content.removeFromTop(visualHeight);
    
    // Place the synth waveform inside the upper-right panel
    auto synthWaveformArea =
        audioVisualBounds.reduced(Layout::panelPadding);

    synthWaveformArea.removeFromTop(
        Layout::titleHeight + Layout::gap);

    // Make the yellow display smaller on all four sides
    synthWaveformArea.reduce(12, 12);

    synthWaveform.setBounds(synthWaveformArea);

    content.removeFromTop(Layout::gap);

    voiceVisualBounds = content;


    // Position the voice waveform

    auto waveformArea =
        voiceVisualBounds.reduced(Layout::panelPadding);


    waveformArea.removeFromTop(
        Layout::titleHeight
        + Layout::rmsRowHeight
        + Layout::centroidRowHeight
        + Layout::flatnessRowHeight
        + Layout::gap);

    voiceWaveform.setBounds(
        waveformArea.reduced(8, 2));
    
    // 6. Oscillator and filter on the first row
    auto topRow =
        controls.removeFromTop(Layout::topRowHeight);

    controls.removeFromTop(Layout::gap);

    const int columnWidth =
        (controls.getWidth() - Layout::gap) / 2;

    osc.setBounds(topRow.removeFromLeft(columnWidth));

    topRow.removeFromLeft(Layout::gap);

    filter.setBounds(topRow);

    // 7. ADSR controls on the second row
    adsr.setBounds(
        controls.removeFromLeft(columnWidth));

    controls.removeFromLeft(Layout::gap);

    modAdsr.setBounds(controls);

    repaint();
    
    
}



void CSC475SynthesizerAudioProcessorEditor::timerCallback()
{
    // Update the control indicators
    osc.setEffectiveFmDepth(
        audioProcessor.effectiveFmDepthForDisplay.load());

    filter.setEffectiveResonance(
        audioProcessor.effectiveResonanceForDisplay.load());

    filter.setEffectiveFilterFrequency(
        audioProcessor.effectiveFilterFrequencyForDisplay.load());

    // 1. LIVE MICROPHONE WAVEFORM
    // Keep using JUCE's AudioVisualiserComponent
    for (int read = 0;
         read < Layout::waveformMaxReadsPerTick;
         ++read)
    {
        const int samplesRead =
            audioProcessor.readWaveformSamples(
                waveformReadBuffer.data(),
                waveformReadBufferSize);

        if (samplesRead == 0)
            break;

        const float* channels[] = {
            waveformReadBuffer.data()
        };

        voiceWaveform.pushBuffer(
            channels, 1, samplesRead);

        if (samplesRead < waveformReadBufferSize)
            break;
    }

    // 2. SYNTH OUTPUT WAVEFORM
    // Send raw audio samples to our custom oscilloscope
    for (int read = 0;
         read < Layout::waveformMaxReadsPerTick;
         ++read)
    {
        const int samplesRead =
            audioProcessor.readSynthWaveformSamples(
                waveformReadBuffer.data(),
                waveformReadBufferSize);

        if (samplesRead == 0)
            break;

        synthWaveform.pushSamples(
            waveformReadBuffer.data(),
            samplesRead);

        if (samplesRead < waveformReadBufferSize)
            break;
    }
    // Update synth oscilloscope every fourth timer tick
    if (++scopeRefreshCounter >= 4)
    {
        scopeRefreshCounter = 0;
        synthWaveform.repaint();
    }

    // Refresh the live microphone readings
    repaint(voiceVisualBounds);
}

