// MainWindow.cpp
// This file contains the implementation of our synthesizer's GUI


#include "include/MainWindow.h"
#include <thread>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <cmath>
#include <mutex>

// Target framerate for smooth UI
constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);

// Initialize the window and GUI components
// This is called when the application starts
void MainWindow::init() {
    // Initialize SDL with video and gamepad support
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
        return;
    }
    // Create a hidden window that we'll show later
    Uint32 window_flags = SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("synth", 584, 868, window_flags);
    if (nullptr == window) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return;
    }
    // Create renderer with VSync enabled for smooth graphics
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (nullptr == renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return;
    }
    // Center the window on screen
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Enable keyboard and gamepad navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsClassic();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Initialize default parameters
    if (audioGenerator) {
        audioGenerator->setOsc1Enabled(osc1_enabled);
        audioGenerator->setOsc2Enabled(osc2_enabled);
        audioGenerator->setOsc3Enabled(osc3_enabled);
        audioGenerator->setOsc1Waveform(static_cast<Oscillator::Waveform>(osc1_waveform));
        audioGenerator->setOsc2Waveform(static_cast<Oscillator::Waveform>(osc2_waveform));
        audioGenerator->setOsc3Waveform(static_cast<Oscillator::Waveform>(osc3_waveform));
        audioGenerator->setOsc1FrequencyOffset(osc1_freq_offset);
        audioGenerator->setOsc2FrequencyOffset(osc2_freq_offset);
        audioGenerator->setOsc3FrequencyOffset(osc3_freq_offset);
    }
    
    if (params) {
        std::lock_guard<std::mutex> lock(params->mutex);
        params->attack = attack_time;
        params->release = release_time;
        params->filter_cutoff = filter_cutoff;
        params->filter_resonance = filter_resonance;

        params->volume = volume;
    }
}

// Main application loop
// Handles events, updates UI, and renders frames
void MainWindow::run() {
    const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool done { false };
    while (!done){
        // Frame timing for consistent 60 FPS
        auto frameStart = std::chrono::high_resolution_clock::now();
        SDL_Event event;
        // Process all pending events
        while (SDL_PollEvent(&event)){
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (SDL_EVENT_QUIT == event.type)
                done = true;
            if ((SDL_EVENT_WINDOW_CLOSE_REQUESTED == event.type)
                && (SDL_GetWindowID(window) == event.window.windowID))
                done = true;
        }
        // Start new ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        draw();
        ImGui::Render();
        // Clear screen and render ImGui
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
        // Frame timing
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;
        if (frameDuration < TARGET_FRAMETIME) {
            std::this_thread::sleep_for(TARGET_FRAMETIME - frameDuration);
        }
    }
    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Draw the synthesizer interface
// This is called every frame
void MainWindow::draw() {
    // Get window dimensions for layout
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    // Set custom colors for the UI
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.61f, 0.39f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.7f, 0.61f, 0.39f, 1.0f));
    ImGui::Begin("Synth", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    

    // Oscillator 1 controls
    ImGui::Spacing();
    ImGui::Checkbox("Oscillator 1", &osc1_enabled);
    ImGui::Text("OSC 1 Waveform");
    const char* waveforms[] = { "Triangle", "Noise", "Saw" };
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::Combo("##osc1_waveform", &osc1_waveform, waveforms, IM_ARRAYSIZE(waveforms)) && audioGenerator) {
        audioGenerator->setOsc1Waveform(static_cast<Oscillator::Waveform>(osc1_waveform));
    }
    ImGui::Text("OSC 1 Frequency Offset");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##freq_offset_osc1", &osc1_freq_offset, -5.0f, 5.0f, "%.3f") && audioGenerator) {
        audioGenerator->setOsc1FrequencyOffset(osc1_freq_offset);
    }
    ImGui::Spacing();


    // Oscillator 2 controls
    ImGui::Spacing();
    ImGui::Checkbox("Oscillator 2", &osc2_enabled);
    ImGui::Text("OSC 2 Waveform");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::Combo("##osc2_waveform", &osc2_waveform, waveforms, IM_ARRAYSIZE(waveforms)) && audioGenerator) {
        audioGenerator->setOsc2Waveform(static_cast<Oscillator::Waveform>(osc2_waveform));
    }
    ImGui::Text("OSC 2 Frequency Offset");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##freq_offset_osc2", &osc2_freq_offset, -5.0f, 5.0f, "%.3f") && audioGenerator) {
        audioGenerator->setOsc2FrequencyOffset(osc2_freq_offset);
    }
    ImGui::Spacing();


    // Oscillator 3 controls
    ImGui::Spacing();
    ImGui::Checkbox("Oscillator 3", &osc3_enabled);
    ImGui::Text("OSC 3 Waveform");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::Combo("##osc3_waveform", &osc3_waveform, waveforms, IM_ARRAYSIZE(waveforms)) && audioGenerator) {
        audioGenerator->setOsc3Waveform(static_cast<Oscillator::Waveform>(osc3_waveform));
    }
    ImGui::Text("OSC 3 Frequency Offset");
    ImGui::SetNextItemWidth(window_width - 40); 
    if (ImGui::SliderFloat("##freq_offset_osc3", &osc3_freq_offset, -5.0f, 5.0f, "%.3f") && audioGenerator) {
        audioGenerator->setOsc3FrequencyOffset(osc3_freq_offset);
    }
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();


    // Envelope controls
    ImGui::Text("Attack");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##attack", &attack_time, 0.0f, 1.0f, "%.3f") && params) {
        std::lock_guard<std::mutex> lock(params->mutex);
        params->attack = attack_time;
    }
    ImGui::Text("Release");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##release", &release_time, 0.0f, 1.0f, "%.3f") && params) {
        std::lock_guard<std::mutex> lock(params->mutex);
        params->release = release_time;
    }

    // Filter controls
    ImGui::Text("Filter Cutoff");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##filter_cutoff", &filter_cutoff, 20.0f, 20000.0f, "%.3f") && params) {
        std::lock_guard<std::mutex> lock(params->mutex);
        params->filter_cutoff = filter_cutoff;
    }
    ImGui::Text("Filter Resonance");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##filter_resonance", &filter_resonance, 0.0f, 0.99f, "%.3f") && params) {
        std::lock_guard<std::mutex> lock(params->mutex);
        params->filter_resonance = filter_resonance;
    }
    
    // Filter LFO controls
    ImGui::Text("Filter Auto-Variation Frequency");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##filter_auto_variation_frequency", &filter_auto_variation_frequency, 1.0f, 20.0f, "%.2f Hz") && params) {
        std::lock_guard<std::mutex> lock(params->mutex);
        params->filter_auto_variation_frequency = filter_auto_variation_frequency;
    }
    
    ImGui::Text("Filter Auto-Variation Amount");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##filter_auto_variation_amount", &filter_auto_variation_amount, 0.0f, 1.0f, "%.2f") && params) {
        std::lock_guard<std::mutex> lock(params->mutex);
        params->filter_auto_variation_amount = filter_auto_variation_amount;
    }


    // Volume control
    ImGui::Text("Volume");
    ImGui::SetNextItemWidth(window_width - 40);
    if (ImGui::SliderFloat("##volume", &volume, 0.0f, 1.0f, "%.3f") && params) {
        std::lock_guard<std::mutex> lock(params->mutex);
        params->volume = volume;
    }

    // Octave control
    // Octave [-2 : +1] : choice of octave for virtual and non-virtual keyboard
    ImGui::Text("Octave");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.71f, 0.49f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.51f, 0.29f, 1.0f));
    if (ImGui::Button("-", ImVec2(30, 20))) {
        if (octave > -2) octave--;
    }
    ImGui::SameLine();
    ImGui::Text("%d", octave);
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(30, 20))) {
        if (octave < 1) octave++;
    }
    ImGui::PopStyleColor(3);
    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    // Piano keyboard layout
    float button_size = 20.0f;
    float spacing = 7.0f;
    float total_width = 13 * (button_size + spacing) - spacing;
    float start_x = (window_width - total_width) * 0.4f;
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::SetCursorPosX(start_x);

    // Keyboard mapping for computer keyboard
    static const ImGuiKey keymap[13] = {
        ImGuiKey_S, ImGuiKey_E, ImGuiKey_D, ImGuiKey_R, ImGuiKey_F, ImGuiKey_G,
        ImGuiKey_Y, ImGuiKey_H, ImGuiKey_U, ImGuiKey_J, ImGuiKey_I, ImGuiKey_K,
        ImGuiKey_L
    };
    static int noteActive = -1;
    ImGuiIO& io = ImGui::GetIO();

    // Handle keyboard input
    for (int i = 0; i < 13; ++i) {
        if (ImGui::IsKeyPressed(keymap[i], false)) {
            if (noteActive != i) {
                handleKeyRelease();
                handleKeyPress(i+1);
                noteActive = i;
            }
        }
        if (ImGui::IsKeyReleased(keymap[i]) && noteActive == i) {
            handleKeyRelease();
            noteActive = -1;
        }
    }

    // Draw piano keyboard buttons
    ImGui::SetCursorPosX(start_x);
    for (int i = 1; i <= 13; i++) {
        if (i > 1) ImGui::SameLine();
        char label[3];
        snprintf(label, sizeof(label), "%d", i);
        bool pressed = ImGui::Button(label, ImVec2(button_size, button_size));
        bool held = ImGui::IsItemActive();
        bool released = ImGui::IsItemDeactivated();
        if ((pressed || held) && noteActive != i-1) {
            handleKeyRelease();
            handleKeyPress(i);
            noteActive = i-1;
        }
        if (released && noteActive == i-1) {
            handleKeyRelease();
            noteActive = -1;
        }
    }
    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::PopStyleColor(3);
}

// Handle note press events
// Called when a key is pressed on the piano keyboard
void MainWindow::handleKeyPress(int key) {
    if (key >= 1 && key <= 13 && audioGenerator && params) {
        int noteNumber = key - 1;
        double freq = AudioGenerator::calculateNoteFrequency(noteNumber, octave);
        {
            std::lock_guard<std::mutex> lock(params->mutex);
            params->frequency = freq;
        }
        audioGenerator->setOsc1Enabled(osc1_enabled);
        audioGenerator->setOsc2Enabled(osc2_enabled);
        audioGenerator->setOsc3Enabled(osc3_enabled);
        audioGenerator->noteOn();
    }
}

// Handle note release events
// Called when a key is released on the piano keyboard
void MainWindow::handleKeyRelease() {
    if (audioGenerator) {
        audioGenerator->noteOff();
    }
}

// Set the audio generator instance
void MainWindow::setAudioGenerator(AudioGenerator* audio) {
    audioGenerator = audio;
}

// Set the synthesizer parameters
void MainWindow::setSynthParams(SynthParams* p) {
    params = p;
}
