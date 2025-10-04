// TripleOscillator class: combines three oscillators and an envelope
// Provides a unified interface for controlling multiple oscillators with a single envelope


#ifndef SIMPLE_SYNTH_TRIPLE_OSCILLATOR_H
#define SIMPLE_SYNTH_TRIPLE_OSCILLATOR_H

#include <mutex>
#include "Oscillator.h"
#include "Envelope.h"

class TripleOscillator {
public:
    // Constructor: initializes osc2 to Saw and osc3 to Noise waveform by default
    TripleOscillator();

    // Set base frequency for all oscillators
    void setFrequency(double freq);

    // Enable/disable oscillators
    void setOsc1Enabled(bool enabled);
    void setOsc2Enabled(bool enabled);
    void setOsc3Enabled(bool enabled);

    // Set waveforms for each oscillator
    void setOsc1Waveform(Oscillator::Waveform wf);
    void setOsc2Waveform(Oscillator::Waveform wf);
    void setOsc3Waveform(Oscillator::Waveform wf);

    // Set frequency offsets for each oscillator
    void setOsc1FrequencyOffset(float offset);
    void setOsc2FrequencyOffset(float offset);
    void setOsc3FrequencyOffset(float offset);

    // Master envelope control methods
    void setAttack(float a);
    void setRelease(float r);
    void noteOn();
    void noteOff();
    void setEnvSampleRate(float sr);

    // Process a buffer of samples (size = bufferSize), combining all oscillators and applying the envelope
    void processBuffer(float* buffer, int bufferSize);

private:
    std::mutex mutex;  // Mutex to protect access to oscillators and envelope
    Oscillator osc1; // First oscillator
    Oscillator osc2; // Second oscillator
    Oscillator osc3; // Third oscillator
    Envelope env;     // Master amplitude envelope
};

#endif //SIMPLE_SYNTH_TRIPLE_OSCILLATOR_H
