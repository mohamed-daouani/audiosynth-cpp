#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H

#include <SDL3/SDL.h>
#include "../../audio/include/AudioGenerator.h"
#include "../../audio/include/SynthParams.h"

// Main window class that handles the synthesizer's GUI
class MainWindow {
public:
    // Constructor initializes all member variables with default values
    MainWindow() : window(nullptr), renderer(nullptr), audioGenerator(nullptr),
                  osc1_enabled(true), osc2_enabled(false), osc3_enabled(false),
                  osc1_waveform(0), osc2_waveform(2), osc3_waveform(1),
                  osc1_freq_offset(0.0f), osc2_freq_offset(0.0f), osc3_freq_offset(0.0f), osc_mix(0.5f), params(nullptr),
                  attack_time(0.5f), release_time(1.0f),
                  filter_cutoff(20000.0f), filter_resonance(0.0f),
                  filter_auto_variation_frequency(10.0f), filter_auto_variation_amount(0.0f),
                  volume(1.0f), isNotePlaying(false), octave(0) {}

    // Initialize the window and GUI components
    void init();
    // Main application loop
    void run();
    // Draw the synthesizer interface
    void draw();
    // Set the audio generator instance
    void setAudioGenerator(AudioGenerator* audio);
    // Set the synthesizer parameters
    void setSynthParams(SynthParams* p);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    AudioGenerator* audioGenerator;
    SynthParams* params;

    bool osc1_enabled;
    bool osc2_enabled;
    bool osc3_enabled;
    int osc1_waveform;
    int osc2_waveform;
    int osc3_waveform;
    float osc1_freq_offset;
    float osc2_freq_offset;
    float osc3_freq_offset;
    float osc_mix;
    float attack_time;
    float release_time;
    float filter_cutoff;
    float filter_resonance;
    float filter_auto_variation_frequency;
    float filter_auto_variation_amount;

    float volume;
    bool isNotePlaying;
    int octave;


    void handleKeyPress(int key);
    void handleKeyRelease();
};

#endif //TESTINSTRUCT_MAINWINDOW_H 