#include "include/Oscillator.h"
#include <cmath>
#include <cstdlib>

// Constructor initializes oscillator parameters
Oscillator::Oscillator() : frequency(440.0), phase(0.0), waveform(Waveform::Triangle), isEnabled(true), frequencyOffset(0.0) {
    std::lock_guard<std::mutex> lock(mutex);
    updatePhaseStep();
}

// Set the oscillator frequency in Hz
void Oscillator::setFrequency(double freq) { 
    std::lock_guard<std::mutex> lock(mutex);
    frequency = freq; 
    updatePhaseStep();
}

// Set the waveform type
void Oscillator::setWaveform(Waveform wf) { 
    std::lock_guard<std::mutex> lock(mutex);
    waveform = wf; 
}

// Enable or disable the oscillator
void Oscillator::setEnabled(bool enabled) { 
    std::lock_guard<std::mutex> lock(mutex);
    isEnabled = enabled; 
}

// Get the current enabled state
bool Oscillator::getEnabled() const { 
    std::lock_guard<std::mutex> lock(mutex);
    return isEnabled; 
}

// Set frequency offset in semitones
void Oscillator::setFrequencyOffset(float offset) {
    std::lock_guard<std::mutex> lock(mutex);
    frequencyOffset = offset;
    updatePhaseStep();
}

// Process a buffer of samples
void Oscillator::processBuffer(float* buffer, int bufferSize) {
    std::lock_guard<std::mutex> lock(mutex);
    for(int i = 0; i < bufferSize; i++) {
        buffer[i] = generateSample();
    }
}

// Phase control methods
double Oscillator::getPhase() const { 
    std::lock_guard<std::mutex> lock(mutex);
    return phase; 
}

void Oscillator::setPhase(double newPhase) { 
    std::lock_guard<std::mutex> lock(mutex);
    phase = newPhase; 
}

// Generate a single sample based on current waveform
float Oscillator::generateSample() {
    if (!isEnabled) return 0.0f;
    
    float sample = 0.0f;
    switch (waveform) {
        case Waveform::Triangle:
            sample = SynthConstants::BASE_AMPLITUDE * static_cast<float>(std::asin(std::sin(phase)) * (2.0 / M_PI));
            phase += phaseStep;
            if (phase >= SynthConstants::TWO_PI) phase -= SynthConstants::TWO_PI;
            break;
        case Waveform::Noise:
            sample = SynthConstants::BASE_AMPLITUDE * (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f);
            break;
        case Waveform::Saw:
            sample = SynthConstants::BASE_AMPLITUDE * static_cast<float>((phase / M_PI) - 1.0);
            phase += phaseStep;
            if (phase >= SynthConstants::TWO_PI) phase -= SynthConstants::TWO_PI;
            break;
    }
    return sample;
}

// Update phase step based on current frequency
void Oscillator::updatePhaseStep() {
    double effectiveFrequency = frequency + frequencyOffset;
    phaseStep = SynthConstants::TWO_PI * effectiveFrequency / SynthConstants::SAMPLE_RATE;
}
