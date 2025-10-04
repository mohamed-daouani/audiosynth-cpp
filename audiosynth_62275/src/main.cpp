#include <iostream>
#include <memory>
#include "audio/include/AudioGenerator.h"
#include "gui/include/MainWindow.h"
#include "audio/include/SynthParams.h"

int main() {
    // Create synthesizer parameters
    SynthParams params;
    // Create main window
    MainWindow mainWindow;
    // Set parameters in main window
    mainWindow.setSynthParams(&params);
    // Create audio generator with parameters
    AudioGenerator* audioGenerator = new AudioGenerator(&params);

    // Initialize window and audio
    mainWindow.init();
    audioGenerator->init();

    // Set audio generator in main window
    mainWindow.setAudioGenerator(audioGenerator);

    // Run the main application loop
    mainWindow.run();

    // Cleanup
    audioGenerator->stop();
    delete audioGenerator;
    return 0;
} 