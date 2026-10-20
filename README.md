# Voice-Controlled Sound Design Synthesizer

CSC 475
Christopher Coetzee (V01001548) and Isabella Nichol (V01007752)
C++ and JUCE

## Description

Sound design is what lies at the core of the fun of synthesizers for a lot of people. The process can be complicated with lots of knobs and dials
and intricacies that are easy to miss. What if there was an easier and more intuitive way to do it? Our synthesizer is an attempt at this. The core
idea of the synth and what makes it unique is that you use your voice to change the way that the synth sounds. The synth is not trying to emulate
your voice but instead uses a creative mapping from elements of your vocal input (amplitude, brightness, frequency) and adjusts the sound accordingly.

## Features and Method

- Polyphonic MIDI synth with sine, sawtooth, and square waves, FM frequency/depth, amplitude and modulation ADSR envelopes, and filter controls.
- Microphone input controls three parameters: RMS level adjusts FM depth, spectral centroid adjusts filter cutoff, and frequency-band energy balance adjusts FM frequency.
- Normalize and smooth these measurements; hold settings during silence.
- Show the voice's live frequency spectrum and the synth's output waveform while playing a preview note.
- Capture Sound freezes the settings for MIDI performance and further manual editing.

Frequency-band balance describes spectral energy, not the sung note. The project does not include voice replication or melody transcription. The core target is a standalone Mac application; effects, preset saving, and DAW input routing are optional.

## Tools, Data, and Testing

Use JUCE/C++, Xcode, GitHub, and Python for development and testing. Use a microphone, headphones, and MIDI keyboard for the demonstration.

Record a small set of vowels, humming, loudness changes, and silence from both team members. Use generated tones and Python calculations to check feature extraction.

Test mapping responsiveness, silence handling, capture stability, MIDI chords, and both displays. Document glitches, limitations, and examples of captured sounds.

## Timeline

Exact dates are to be confirmed.

- Weeks 1–2: Confirm requirements; stabilize the synth, implement input analysis, and prototype the interface.
- Weeks 3–4: Integrate mappings, visualizations, preview, and capture; update the progress report.
- Week 5: Test the complete workflow and fix issues.
- Week 6: Finish the implementation, report, and presentation.

Deliverables: design specification (10%), progress report (5%), final report (20%), implementation (15%), and presentation (5%).

## Work Division

Christopher: synthesis, MIDI, microphone analysis, mappings, smoothing, and capture.

Bella: interface controls, both visualizations, test recordings, offline checks, and report/presentation assembly.

## Christopher Coetzee

Objective 1: Build the polyphonic synth.

- PI1 (basic): Generate selectable sine, sawtooth, and square notes from MIDI.
- PI2 (basic): Implement FM and amplitude ADSR controls.
- PI3 (expected): Implement filter controls and modulation ADSR.
- PI4 (expected): Support chords and repeated notes with correct releases and controlled output levels.
- PI5 (advanced): Add one effect with bypass.

Objective 2: Implement voice control and capture.

- PI1 (basic): Capture microphone input and calculate RMS.
- PI2 (basic): Calculate spectral centroid and frequency-band energy balance.
- PI3 (expected): Implement three bounded, smoothed mappings with silence handling.
- PI4 (expected): Implement preview and capture; verify captured settings stay fixed during MIDI playback.
- PI5 (advanced): Add adjustable mapping sensitivity or ranges.

## Isabella Nichol

Objective 1: Build the interface and visualizations.

- PI1 (basic): Arrange and connect the synth controls.
- PI2 (basic): Add voice-design, preview, and capture buttons with mode status.
- PI3 (expected): Display the live input spectrum with labeled axes.
- PI4 (expected): Display the synth waveform and keep both displays responsive.
- PI5 (advanced): Visualize the three feature-to-parameter mappings.

Objective 2: Test and document the workflow.

- PI1 (basic): Prepare recordings and a test checklist.
- PI2 (basic): Check selected feature values against Python calculations.
- PI3 (expected): Test mappings, silence, capture, and MIDI playback; document and retest failures.
- PI4 (expected): Complete the integrated test and assemble results, instructions, and presentation materials.
- PI5 (advanced): Compare sound-design sessions with and without visual feedback.

## Related Work

Fasciani and Wyse [1] study voice control of sound generators. SynthAssist [2] uses vocal imitation to search for synth settings; our system instead maps features directly and captures the resulting patch. Hunt et al. [3] inform our mapping design. Tzanetakis and Cook [4] provide background on audio descriptors. The Timbre Explorer [5] connects synthesis controls with live visual feedback. Shier et al. [6] explore mapping input timbre onto synthesizer controls. West et al. [7] discuss criteria for understandable and useful mappings. Gregorio and Kim [8] evaluate timbre-based synth controls, informing our evaluation approach. Kleinberger et al. [9] review voice-based musical interfaces, providing broader context for our project. Fiebrink et al. [10] introduce learning-based mappings through Wekinator, offering a comparison with our explicitly programmed mappings. Chowning [11] provides the foundation for FM synthesis and explains how modulation shapes a sound’s frequency content. Peeters [12] describes audio features that inform our measurement of vocal amplitude, brightness, and frequency-band energy. McPherson et al. [13] examine action-to-sound latency, informing our testing of responsiveness between vocal input and synth changes. Wanderley and Orio [14] discuss methods for evaluating musical input devices, supporting our assessment of voice-based control. McFee et al. [15] describe librosa, providing a reference for Python-based audio analysis and verification of our feature calculations.

## References

1. Fasciani, S., and Wyse, L. (2012). A Voice Interface for Sound Generators: Adaptive and Automatic Mapping of Gestures to Sound. NIME. 
    https://www.nime.org/proceedings/2012/nime2012_57.pdf

2. Cartwright, M., and Pardo, B. (2014). SynthAssist: Querying an Audio Synthesizer by Vocal Imitation. NIME, 363–366. 
    https://doi.org/10.5281/zenodo.1178730

3. Hunt, A. D., Wanderley, M. M., and Paradis, M. (2002). The Importance of Parameter Mapping in Electronic Instrument Design. NIME, 88–93. 
    https://doi.org/10.5281/zenodo.1176424

4. Tzanetakis, G., and Cook, P. (2002). Musical Genre Classification of Audio Signals. IEEE Transactions on Speech and Audio Processing, 10(5), 293–302. 
    https://doi.org/10.1109/TSA.2002.800560

5. Lam, J. R., and Saitis, C. (2021). The Timbre Explorer: A Synthesizer Interface for Educational Purposes and Perceptual Studies. NIME, article 62. 
    https://doi.org/10.21428/92fbeb44.92a95683

6. Shier, J., Saitis, C., Robertson, A., and McPherson, A. (2024). Real-time Timbre Remapping with Differentiable DSP. Proceedings of the International Conference on New Interfaces for Musical Expression, 377–385. https://doi.org/10.5281/zenodo.13904884

7. West, T., Caramiaux, B., Huot, S., and Wanderley, M. M. (2021). Making Mappings: Design Criteria for Live Performance. Proceedings of the International Conference on New Interfaces for Musical Expression, article 31. https://doi.org/10.21428/92fbeb44.04f0fc35

8. Gregorio, J., and Kim, Y. E. (2021). Evaluation of Timbre-Based Control of a Parametric Synthesizer. Proceedings of the International Conference on New Interfaces for Musical Expression, article 83. https://doi.org/10.21428/92fbeb44.31419bf9

9. Kleinberger, R., Singh, N., Xiao, X., and van Troyer, A. (2022). Voice at NIME: A Taxonomy of New Interfaces for Vocal Musical Expression. Proceedings of the International Conference on New Interfaces for Musical Expression. https://doi.org/10.21428/92fbeb44.4308fb94

10. Fiebrink, R., Trueman, D., and Cook, P. R. (2009). A Meta-Instrument for Interactive, On-the-fly Machine Learning. Proceedings of the International Conference on New Interfaces for Musical Expression, 280–285. https://www.nime.org/proceedings/2009/nime2009_280.pdf

11. Chowning, J. M. (1973). The Synthesis of Complex Audio Spectra by Means of Frequency Modulation. Journal of the Audio Engineering Society, 21(7), 526–534.
    https://aes.org/publications/elibrary-page/?id=1954
    
12. Peeters, G. (2004). A Large Set of Audio Features for Sound Description (Similarity and Classification) in the CUIDADO Project. Technical report, IRCAM.
    https://discussion.forum.ircam.fr/uploads/default/original/1X/ffa6a82f823873f864681994c271fb157e41a627.pdf
    
13. McPherson, A., Jack, R., and Moro, G. (2016). Action-Sound Latency: Are Our Tools Fast Enough? Proceedings of the International Conference on New Interfaces for Musical Expression, 20–25.
    https://doi.org/10.5281/zenodo.3964611

14. Wanderley, M. M., and Orio, N. (2002). Evaluation of Input Devices for Musical Expression: Borrowing Tools from HCI. Computer Music Journal, 26(3), 62–76.
    https://doi.org/10.1162/014892602320582981
    
15. McFee, B., Raffel, C., Liang, D., Ellis, D. P. W., McVicar, M., Battenberg, E., and Nieto, O. (2015). librosa: Audio and Music Signal Analysis in Python. Proceedings of the 14th Python in Science Conference, 18–24.
    https://doi.org/10.25080/Majora-7b98e3ed-003
