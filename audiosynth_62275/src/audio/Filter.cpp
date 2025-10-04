#include "include/Filter.h"
#include <cmath>

// Constructor initializing sample rate and default parameters
LowPassFilter::LowPassFilter(float sampleRate)
    : sampleRate(sampleRate), 
      cutoff(20000.0f), // Default cutoff frequency: 20kHz (essentially bypass)
      resonance(0.0f),  // Default resonance (no resonance)
      lfoFrequency(0.0f), // Default LFO frequency: 0 Hz (no modulation)
      lfoAmount(0.0f),    // Default LFO amount: 0 (no modulation)
      lfoPhase(0.0f),     // Initial LFO phase: 0
      baseCutoff(20000.0f), // Base cutoff frequency
      x1(0.0f), 
      x2(0.0f), 
      y1(0.0f), 
      y2(0.0f) {
    updateCoefficients();
}

// Set a new cutoff frequency (in Hz)
void LowPassFilter::setCutoff(float newCutoff) {
    std::lock_guard<std::mutex> lock(mutex); // Thread-safe update
    baseCutoff = newCutoff; // Save the base cutoff frequency
    updateCutoffWithLFO(); // Apply LFO modulation
}

// Set a new resonance (usually between 0.0 and 1.0)
void LowPassFilter::setResonance(float newResonance) {
    std::lock_guard<std::mutex> lock(mutex); // Thread-safe update
    resonance = newResonance;
    updateCoefficients(); // Recalculate filter coefficients
}

// Set LFO frequency for automatic cutoff variation
void LowPassFilter::setAutoVariationFrequency(float frequency) {
    std::lock_guard<std::mutex> lock(mutex);
    lfoFrequency = frequency;
    updateCutoffWithLFO();
}

// Set LFO amount for automatic cutoff variation
void LowPassFilter::setAutoVariationAmount(float amount) {
    std::lock_guard<std::mutex> lock(mutex);
    lfoAmount = amount;
    updateCutoffWithLFO();
}

// Reset filter history (clear previous input/output samples)
void LowPassFilter::reset() {
    std::lock_guard<std::mutex> lock(mutex); // Thread-safe reset
    x1 = x2 = y1 = y2 = 0.0f;
    lfoPhase = 0.0f; // Reset LFO phase
}

// Process a single input sample and return the filtered output
float LowPassFilter::process(float input) {
    std::lock_guard<std::mutex> lock(mutex); // Thread-safe processing
    
    // Update LFO phase and apply modulation if enabled
    if (lfoFrequency >= 1.0f && lfoAmount > 0.0f) {
        updateLFO();
    }
    
    // Apply the difference equation of the biquad filter
    float output = a0 * input + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;

    // Shift history for next sample
    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;

    return output;
}

// Recalculate filter coefficients based on cutoff and resonance
void LowPassFilter::updateCoefficients() {
    // Convert resonance to Q factor
    float q = 0.5f / (1.0f - resonance);

    // Pre-warped angular frequency
    float omega = 2.0f * M_PI * cutoff / sampleRate;

    // Bandwidth parameter
    float alpha = sin(omega) / (2.0f * q);

    float cosw = cos(omega);

    // Normalization factor
    float norm = 1.0f / (1.0f + alpha);

    // Low-pass filter coefficients (Direct Form 1 biquad)
    a0 = (1.0f - cosw) * 0.5f * norm;
    a1 = (1.0f - cosw) * norm;
    a2 = (1.0f - cosw) * 0.5f * norm;
    b1 = -2.0f * cosw * norm;
    b2 = (1.0f - alpha) * norm;
}

// Update LFO phase and apply modulation to cutoff frequency
void LowPassFilter::updateLFO() {
    // Update LFO phase
    lfoPhase += 2.0f * M_PI * lfoFrequency / sampleRate;
    if (lfoPhase >= 2.0f * M_PI) {
        lfoPhase -= 2.0f * M_PI;
    }
    
    // Calculate LFO modulation value (sine wave)
    float lfoValue = sin(lfoPhase);
    
    // Apply modulation to cutoff frequency
    // Amount of 1.0 gives ±5000 Hz variation around base frequency
    float modulation = lfoValue * lfoAmount * 5000.0f;
    cutoff = baseCutoff + modulation;
    
    // Clamp cutoff frequency to valid range (20 Hz to 20 kHz)
    cutoff = std::max(20.0f, std::min(20000.0f, cutoff));
    
    // Update filter coefficients with new cutoff frequency
    updateCoefficients();
}

// Update cutoff frequency with current LFO modulation
void LowPassFilter::updateCutoffWithLFO() {
    if (lfoFrequency >= 1.0f && lfoAmount > 0.0f) {
        // Apply current LFO modulation
        updateLFO();
    } else {
        // No LFO modulation, use base cutoff frequency
        cutoff = baseCutoff;
        updateCoefficients();
    }
}
