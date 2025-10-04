
#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <mutex>

// Simple envelope generator with attack and release stages
class Envelope {
public:
    Envelope() = default;
    // Set attack time in seconds
    void setAttack(float a);
    // Set release time in seconds
    void setRelease(float r);
    // Start a new note
    void noteOn();
    // Release the current note
    void noteOff();
    // Process the envelope and return current amplitude
    float process();
    // Set the sample rate for timing calculations
    void setSampleRate(float sr);
private:
    std::mutex mutex;  // Mutex to protect access to parameters
    float attack = 0.01f;      // Attack time in seconds
    float release = 0.1f;      // Release time in seconds
    float envelope = 0.0f;     // Current envelope value
    bool gate = false;         // Note on/off state
    float sampleRate = 44100.0f; // Sample rate for timing
};

#endif // ENVELOPE_H 