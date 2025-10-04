#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

#include <mutex>

// Simple Biquad Low-pass filter implementation
class LowPassFilter {
public:
    // Constructor initializing sample rate and default parameters
    LowPassFilter(float sampleRate);

    // Set a new cutoff frequency (in Hz)
    void setCutoff(float newCutoff);

    // Set a new resonance (usually between 0.0 and 1.0)
    void setResonance(float newResonance);

    // Set LFO parameters for automatic cutoff variation
    void setAutoVariationFrequency(float frequency);
    void setAutoVariationAmount(float amount);

    // Reset filter history (clear previous input/output samples)
    void reset();

    // Process a single input sample and return the filtered output
    float process(float input);

private:
    // Recalculate filter coefficients based on cutoff and resonance
    void updateCoefficients();
    
    // Update LFO phase and apply modulation to cutoff frequency
    void updateLFO();
    
    // Update cutoff frequency with current LFO modulation
    void updateCutoffWithLFO();

    // Filter parameters
    float sampleRate;  // Sampling rate (Hz)
    float cutoff;      // Cutoff frequency (Hz)
    float resonance;   // Resonance amount (mapped to Q)
    
    // LFO parameters for automatic cutoff variation
    float lfoFrequency;    // LFO frequency in Hz
    float lfoAmount;       // LFO amount (0.0 to 1.0)
    float lfoPhase;        // Current LFO phase (0.0 to 2π)
    float baseCutoff;      // Base cutoff frequency (without LFO modulation)

    // Filter coefficients
    float a0, a1, a2, b1, b2;

    // Filter state (history of inputs and outputs)
    float x1, x2;  // Previous inputs
    float y1, y2;  // Previous outputs

    // Mutex for thread-safe operations
    std::mutex mutex;
};

#endif // LOWPASS_FILTER_H
