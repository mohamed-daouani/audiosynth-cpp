#include "include/Envelope.h"

// Set attack time in seconds
void Envelope::setAttack(float a) {
    std::lock_guard<std::mutex> lock(mutex);
    attack = a;
}

// Set release time in seconds
void Envelope::setRelease(float r) {
    std::lock_guard<std::mutex> lock(mutex);
    release = r;
}

// Start a new note
void Envelope::noteOn() {
    std::lock_guard<std::mutex> lock(mutex);
    gate = true;
}

// Release the current note
void Envelope::noteOff() {
    std::lock_guard<std::mutex> lock(mutex);
    gate = false;
}

// Process the envelope and return current amplitude
float Envelope::process() {
    std::lock_guard<std::mutex> lock(mutex);
    float attackStep = (attack > 0.0f) ? (1.0f / (attack * sampleRate)) : 1.0f;
    float releaseStep = (release > 0.0f) ? (1.0f / (release * sampleRate)) : 1.0f;
    
    if (gate) {
        envelope += attackStep;
        if (envelope > 1.0f) envelope = 1.0f;
    } else {
        envelope -= releaseStep;
        if (envelope < 0.0f) envelope = 0.0f;
    }
    return envelope;
}

// Set the sample rate for timing calculations
void Envelope::setSampleRate(float sr) {
    std::lock_guard<std::mutex> lock(mutex);
    sampleRate = sr;
}
