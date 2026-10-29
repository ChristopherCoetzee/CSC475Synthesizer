#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    namespace Layout
    {
        constexpr int editorWidth = 1100;
        constexpr int editorHeight = 800;

        constexpr int margin = 16;
        constexpr int gap = 12;

        constexpr int controlsWidth = 650;
        constexpr int topRowHeight = 260;

        constexpr int inputControlsHeight = 64;
        constexpr int midiHeight = 120;

        constexpr int panelPadding = 12;
        constexpr int titleHeight = 24;
        constexpr int borderWidth = 1;

        constexpr float titleFontSize = 17.0f;
        constexpr float noteFontSize = 14.0f;
        
        constexpr int meterRefreshHz = 20;
        constexpr int rmsDecimalPlaces = 5;
    
        // these are for the input voices waveform
    
        constexpr int waveFormHistorySize = 512;
        constexpr int waveformSamplesPerPoint = 128;
        constexpr int waveformMaxReadsPerTick = 8;
        constexpr int rmsRowHeight = 22;
    
    
    }

    void drawReservedPanel (
        juce::Graphics& g,
        juce::Rectangle<int> bounds,
        const juce::String& title,
        const juce::String& note)
    {
        g.setColour (juce::Colours::darkgrey);
        g.drawRect (bounds, Layout::borderWidth);

        auto content = bounds.reduced (Layout::panelPadding);

        g.setColour (juce::Colours::white);
        g.setFont (
            juce::Font (juce::FontOptions (Layout::titleFontSize)));

        g.drawText (
            title,
            content.removeFromTop (Layout::titleHeight),
            juce::Justification::centredLeft);

        g.setColour (juce::Colours::lightgrey);
        g.setFont (
            juce::Font (juce::FontOptions (Layout::noteFontSize)));

        g.drawFittedText (
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
    const auto background = juce::Colours::black;
    const auto foreground = juce::Colours::white;
    const auto outline = juce::Colours::darkgrey;
    const auto accent = juce::Colours::lightgrey;

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

    addAndMakeVisible (osc);
    addAndMakeVisible (filter);
    addAndMakeVisible (adsr);
    addAndMakeVisible (modAdsr);
    
    voiceWaveform.setSamplesPerBlock(Layout::waveformSamplesPerPoint);
    voiceWaveform.setBufferSize(Layout::waveFormHistorySize);
    voiceWaveform.setColours (
        juce::Colour (0xff100818),
        juce::Colour (0xffc084fc));
    
    voiceWaveform.setRepaintRate(Layout::meterRefreshHz);
    addAndMakeVisible(voiceWaveform);

    setSize (Layout::editorWidth, Layout::editorHeight);
    
    startTimerHz(Layout::meterRefreshHz);
}

CSC475SynthesizerAudioProcessorEditor::
~CSC475SynthesizerAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
}

void CSC475SynthesizerAudioProcessorEditor::paint (
    juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    drawReservedPanel (
        g,
        audioVisualBounds,
        "Audio transformation visual",
        "Reserved for displaying how parameters change the audio.");

    drawReservedPanel (
        g,
        voiceVisualBounds,
        "Live voice input waveform",
        "");

    auto rmsArea =
        voiceVisualBounds.reduced (Layout::panelPadding);

    rmsArea.removeFromTop (Layout::titleHeight);

    const float currentInputRms = audioProcessor.inputRms.load();

    g.setColour (juce::Colours::lightgrey);
    g.setFont (
        juce::Font (juce::FontOptions (Layout::noteFontSize)));

    g.drawText (
        juce::String ("Input RMS: ")
            + juce::String (currentInputRms, Layout::rmsDecimalPlaces),
        rmsArea.removeFromTop (Layout::rmsRowHeight),
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
    auto content = getLocalBounds().reduced (Layout::margin);

    // Full-width reserved areas at the bottom.
    midiBounds = content.removeFromBottom (Layout::midiHeight);
    content.removeFromBottom (Layout::gap);

    inputControlsBounds =
        content.removeFromBottom (Layout::inputControlsHeight);

    content.removeFromBottom (Layout::gap);

    // Left: synth controls. Right: future visual displays.
    auto controls = content.removeFromLeft (Layout::controlsWidth);
    content.removeFromLeft (Layout::gap);

    const auto visualHeight =
        (content.getHeight() - Layout::gap) / 2;

    audioVisualBounds = content.removeFromTop (visualHeight);
    content.removeFromTop (Layout::gap);
    voiceVisualBounds = content;
    
    auto waveformArea = voiceVisualBounds.reduced (Layout::panelPadding);
    
    waveformArea.removeFromTop(Layout::titleHeight + Layout::rmsRowHeight + Layout::gap);
    
    voiceWaveform.setBounds(waveformArea);

    // Top control row: FM modulation and filter.
    auto topRow = controls.removeFromTop (Layout::topRowHeight);
    controls.removeFromTop (Layout::gap);

    const auto columnWidth =
        (controls.getWidth() - Layout::gap) / 2;

    osc.setBounds (topRow.removeFromLeft (columnWidth));
    topRow.removeFromLeft (Layout::gap);
    filter.setBounds (topRow);

    // Lower control row: amplitude ADSR and modulation ADSR.
    adsr.setBounds (controls.removeFromLeft (columnWidth));
    controls.removeFromLeft (Layout::gap);
    modAdsr.setBounds (controls);

    repaint();
}

void CSC475SynthesizerAudioProcessorEditor::timerCallback()
{
    osc.setEffectiveFmDepth(
        audioProcessor.effectiveFmDepthForDisplay.load());
    
    for (int read = 0; read < Layout::waveformMaxReadsPerTick; ++read)
    {
        const int samplesRead = audioProcessor.readWaveformSamples(waveformReadBuffer.data(),waveformReadBufferSize);
        
        if (samplesRead == 0)
            break;
        
        const float* channels[] = { waveformReadBuffer.data() };
        voiceWaveform.pushBuffer(channels, 1, samplesRead);
        
        if (samplesRead < waveformReadBufferSize)
            break;
    }
    
    repaint(voiceVisualBounds);
}
