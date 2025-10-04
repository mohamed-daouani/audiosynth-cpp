// Oscillator.h
// Implements a basic oscillator for the synthesizer


#ifndef SIMPLE_SYNTH_OSCILLATOR_H
#define SIMPLE_SYNTH_OSCILLATOR_H

#include <cmath>
#include <mutex>
#include "SynthConstants.h"

// Oscillator class that generates different waveforms
class Oscillator {
public:
    // Available waveform types
    enum class Waveform {
        Triangle,    // Triangle wave
        Noise,       // Noise wave
        Saw          // Sawtooth wave
    };

    // Constructor initializes oscillator parameters
    Oscillator();

    // Set the oscillator frequency in Hz
    void setFrequency(double freq);
    
    // Set the waveform type
    void setWaveform(Waveform wf);
    // Enable or disable the oscillator
    void setEnabled(bool enabled);
    // Get the current enabled state
    bool getEnabled() const;
    
    // Set frequency offset in semitones
    void setFrequencyOffset(float offset);

    // Process a buffer of samples
    void processBuffer(float* buffer, int bufferSize);

    // Phase control methods
    double getPhase() const;
    void setPhase(double newPhase);

private:
    // Generate a single sample based on current waveform
    float generateSample();

    // Update phase step based on current frequency
    void updatePhaseStep();

    mutable std::mutex mutex;       // Mutex to protect access to parameters
    double frequency;        // Base frequency in Hz
    double phase;           // Current phase (0.0 to 2π)
    double phaseStep;       // Phase increment per sample
    Waveform waveform;      // Current waveform type
    bool isEnabled;         // Oscillator enabled state
    float frequencyOffset;  // Frequency offset in semitones
};

#endif //SIMPLE_SYNTH_OSCILLATOR_H 