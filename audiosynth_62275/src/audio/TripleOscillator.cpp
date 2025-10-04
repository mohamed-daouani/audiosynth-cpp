#include "include/TripleOscillator.h"

// Constructor
TripleOscillator::TripleOscillator() {
    std::lock_guard<std::mutex> lock(mutex);

    // Set default waveforms
    osc1.setWaveform(Oscillator::Waveform::Triangle);  // Default: Triangle
    osc2.setWaveform(Oscillator::Waveform::Saw);       // Default: Saw
    osc3.setWaveform(Oscillator::Waveform::Noise);     // Default: Noise
    
    // Enable only osc1 by default, others stay disabled
    osc1.setEnabled(true);
    osc2.setEnabled(false);
    osc3.setEnabled(false);
}

// Set base frequency for all oscillators
void TripleOscillator::setFrequency(double freq) {
    std::lock_guard<std::mutex> lock(mutex);
    osc1.setFrequency(freq);
    osc2.setFrequency(freq);
    osc3.setFrequency(freq);
}

// Enable/disable first oscillator
void TripleOscillator::setOsc1Enabled(bool enabled) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc1.setEnabled(enabled); 
}

// Enable/disable second oscillator
void TripleOscillator::setOsc2Enabled(bool enabled) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc2.setEnabled(enabled); 
}

// Enable/disable third oscillator
void TripleOscillator::setOsc3Enabled(bool enabled) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc3.setEnabled(enabled); 
}

// Set waveform of first oscillator
void TripleOscillator::setOsc1Waveform(Oscillator::Waveform wf) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc1.setWaveform(wf); 
}

// Set waveform of second oscillator
void TripleOscillator::setOsc2Waveform(Oscillator::Waveform wf) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc2.setWaveform(wf); 
}

// Set waveform of third oscillator
void TripleOscillator::setOsc3Waveform(Oscillator::Waveform wf) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc3.setWaveform(wf); 
}

// Set frequency offset for first oscillator
void TripleOscillator::setOsc1FrequencyOffset(float offset) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc1.setFrequencyOffset(offset); 
}

// Set frequency offset for second oscillator
void TripleOscillator::setOsc2FrequencyOffset(float offset) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc2.setFrequencyOffset(offset); 
}

// Set frequency offset for third oscillator
void TripleOscillator::setOsc3FrequencyOffset(float offset) { 
    std::lock_guard<std::mutex> lock(mutex);
    osc3.setFrequencyOffset(offset); 
}

// Set attack time for the envelope
void TripleOscillator::setAttack(float a) {
    std::lock_guard<std::mutex> lock(mutex);
    env.setAttack(a); 
}

// Set release time for the envelope
void TripleOscillator::setRelease(float r) {
    std::lock_guard<std::mutex> lock(mutex);
    env.setRelease(r); 
}

// Trigger note-on for the envelope
void TripleOscillator::noteOn() { 
    std::lock_guard<std::mutex> lock(mutex);
    env.noteOn(); 
}

// Trigger note-off for the envelope
void TripleOscillator::noteOff() { 
    std::lock_guard<std::mutex> lock(mutex);
    env.noteOff(); 
}

// Set sample rate for the envelope
void TripleOscillator::setEnvSampleRate(float sr) {
    std::lock_guard<std::mutex> lock(mutex);
    env.setSampleRate(sr); 
}

// Process a buffer of samples (size = bufferSize), combining all oscillators and applying the envelope
void TripleOscillator::processBuffer(float* buffer, int bufferSize) {
    std::lock_guard<std::mutex> lock(mutex);
    float tempBuffer1[bufferSize]; // Temp buffer for osc1
    float tempBuffer2[bufferSize]; // Temp buffer for osc2
    float tempBuffer3[bufferSize]; // Temp buffer for osc3

    // Process all oscillators into their respective buffers
    osc1.processBuffer(tempBuffer1, bufferSize);
    osc2.processBuffer(tempBuffer2, bufferSize);
    osc3.processBuffer(tempBuffer3, bufferSize);

    // Mix the three oscillators and apply the master amplitude envelope
    for(int i = 0; i < bufferSize; i++) {
        float envValue = env.process(); // Get master envelope value for this sample

        buffer[i] = (tempBuffer1[i] + tempBuffer2[i] + tempBuffer3[i]) * envValue;
    }
}
