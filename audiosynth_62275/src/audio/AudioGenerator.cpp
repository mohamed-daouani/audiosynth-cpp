#include "include/AudioGenerator.h"
#include <cmath>

constexpr int FRAMES_PER_BUFFER = 256;

// Constructor: initializes synth modules with sample rate
AudioGenerator::AudioGenerator(SynthParams* params) 
    : stream(nullptr), 
      params(params), 
      filter(SynthConstants::SAMPLE_RATE) {}

// Destructor: ensures audio stream is stopped
AudioGenerator::~AudioGenerator() { 
    stop(); 
}

// Initialize audio stream
void AudioGenerator::init() {
    std::lock_guard<std::mutex> lock(mutex);
    
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        return;
    }

    // Open default audio output stream (stereo, 32-bit float, with callback)
    err = Pa_OpenDefaultStream(&stream,
                              0,          // No input channels
                              2,          // Stereo output (2 channels)
                              paFloat32,  // 32-bit float audio format
                              SynthConstants::SAMPLE_RATE,
                              FRAMES_PER_BUFFER,
                              audioCallback,
                              this);      // Pass this object to callback

    if (err != paNoError) {
        return;
    }

    // Start audio stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        return;
    }
}

// Stop audio stream and clean up
void AudioGenerator::stop() {
    std::lock_guard<std::mutex> lock(mutex);
    
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        stream = nullptr;
    }
    Pa_Terminate();
}

// Synth parameter setters: forward calls to TripleOscillator with thread-safety
void AudioGenerator::setFrequency(double freq) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setFrequency(freq); 
}

void AudioGenerator::setOsc1Enabled(bool enabled) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc1Enabled(enabled); 
}

void AudioGenerator::setOsc1Waveform(Oscillator::Waveform wf) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc1Waveform(wf); 
}

void AudioGenerator::setOsc1FrequencyOffset(float offset) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc1FrequencyOffset(offset); 
}

void AudioGenerator::setOsc2Enabled(bool enabled) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc2Enabled(enabled); 
}

void AudioGenerator::setOsc2Waveform(Oscillator::Waveform wf) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc2Waveform(wf); 
}

void AudioGenerator::setOsc2FrequencyOffset(float offset) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc2FrequencyOffset(offset); 
}

void AudioGenerator::setOsc3Enabled(bool enabled) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc3Enabled(enabled); 
}

void AudioGenerator::setOsc3Waveform(Oscillator::Waveform wf) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc3Waveform(wf); 
}

void AudioGenerator::setOsc3FrequencyOffset(float offset) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setOsc3FrequencyOffset(offset); 
}

void AudioGenerator::setAttack(float a) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setAttack(a); 
}

void AudioGenerator::setRelease(float r) { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.setRelease(r); 
}

void AudioGenerator::noteOn() { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.noteOn(); 
}

void AudioGenerator::noteOff() { 
    std::lock_guard<std::mutex> lock(mutex);
    oscillator.noteOff(); 
}

// Implementation of static utility function
double AudioGenerator::calculateNoteFrequency(int noteNumber, int octave) {
    // Calculate frequency using the formula: 220 * 2^(octave + numeroNote/12.0)
    // where numeroNote varies from 0 to 12 and octave varies between -2 and +1
    return 220.0 * std::pow(2.0, octave + noteNumber / 12.0);
}

// PortAudio callback function (called repeatedly to fill audio buffer)
int AudioGenerator::audioCallback(const void *inputBuffer, 
                               void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData) {
    // Cast userData to AudioGenerator*
    auto* generator = static_cast<AudioGenerator*>(userData);
    auto* out = static_cast<float*>(outputBuffer); // Output buffer
    float buffer[FRAMES_PER_BUFFER];               // Temporary mono buffer

    // Default values (will be updated from params)
    float filterCutoff = 20000.0f;
    float filterResonance = 0.0f;
    float filterAutoVariationFreq = 0.0f;
    float filterAutoVariationAmount = 0.0f;

    float volume = 1.0f;

    // Read synth parameters safely (protected by mutex)
    {
        std::lock_guard<std::mutex> lock(generator->params->mutex);

        // Update oscillator and envelope parameters
        generator->oscillator.setFrequency(generator->params->frequency);
        generator->oscillator.setAttack(generator->params->attack);
        generator->oscillator.setRelease(generator->params->release);
        generator->oscillator.setEnvSampleRate(SynthConstants::SAMPLE_RATE);

        // Get filter and volume parameters
        filterCutoff = generator->params->filter_cutoff;
        filterResonance = generator->params->filter_resonance;
        filterAutoVariationFreq = generator->params->filter_auto_variation_frequency;
        filterAutoVariationAmount = generator->params->filter_auto_variation_amount;
        volume = generator->params->volume;
    }

    // Process oscillators: generate raw audio buffer
    generator->oscillator.processBuffer(buffer, FRAMES_PER_BUFFER);

    // Apply low-pass filter parameters
    generator->filter.setCutoff(filterCutoff);
    generator->filter.setResonance(filterResonance);
    generator->filter.setAutoVariationFrequency(filterAutoVariationFreq);
    generator->filter.setAutoVariationAmount(filterAutoVariationAmount);

    // Process each sample through the effects chain
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        float sample = buffer[i];
        
        // Apply filter
        sample = generator->filter.process(sample);
        

        
        // Apply volume control
        sample = sample * volume;
        
        // Convert to stereo (duplicate mono to both channels)
        out[i * 2] = sample;     // Left channel
        out[i * 2 + 1] = sample; // Right channel
    }

    return paContinue;
}
