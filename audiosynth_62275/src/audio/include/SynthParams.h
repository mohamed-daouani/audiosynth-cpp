#ifndef TESTINSTRUCT_SYNTHPARAMS_H
#define TESTINSTRUCT_SYNTHPARAMS_H

#include <mutex>

// Structure to hold all synthesizer parameters
// Protected by a mutex for thread-safe access
struct SynthParams {
    std::mutex mutex;
    
    // Base frequency parameters
    double frequency { 440.0 };  // Base frequency in Hz
    
    // Envelope parameters
    float attack { 0.1f };       // Envelope attack time in seconds
    float release { 0.5f };      // Envelope release time in seconds
    
    // Filter parameters
    float filter_cutoff { 20000.0f };  // Filter cutoff frequency in Hz
    float filter_resonance { 0.0f };   // Filter resonance (0.0 to 0.99)
    float filter_auto_variation_frequency { 10.0f };  // LFO frequency for filter cutoff modulation (Hz)
    float filter_auto_variation_amount { 0.0f };     // LFO amount for filter cutoff modulation (0.0 to 1.0)
    
    // Volume parameter
    float volume { 1.0f };       // Master volume (0.0 to 1.0)
    
    // Oscillator parameters
    int osc1_waveform { 0 };     // Oscillator 1 waveform (0=Sine, 1=Square, 2=Saw)
    int osc2_waveform { 2 };     // Oscillator 2 waveform (0=Sine, 1=Square, 2=Saw)
    int osc3_waveform { 1 };     // Oscillator 3 waveform (0=Sine, 1=Square, 2=Saw)
    bool osc1_enabled { true };  // Oscillator 1 enabled state
    bool osc2_enabled { false }; // Oscillator 2 enabled state
    bool osc3_enabled { false }; // Oscillator 3 enabled state
    float osc1_frequency_offset { 0.0f };  // Oscillator 1 frequency offset in semitones
    float osc2_frequency_offset { 0.0f };  // Oscillator 2 frequency offset in semitones
    float osc3_frequency_offset { 0.0f };  // Oscillator 3 frequency offset in semitones
    
    // Mixing parameters
    float osc_mix { 0.5f };      // Mix between oscillators (0.0 to 1.0)
};

#endif //TESTINSTRUCT_SYNTHPARAMS_H 