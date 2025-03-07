#include <wx/wx.h> //GUI library
#include "RtAudio.h" //Audio library
#include <thread> //This lets you create a new thread to run the audio independent of the GUI
#include <atomic>  //Allows you to create atomic variables. Atomic variables are special variables that can be read and written to by multiple threads without causing race conditions.
#include "VoiceFunctions.h" //This header file contains the synth voice class. It will contain the oscillator, envelope, and filter methods that will be used throughout the code.
#include <windows.h>
#include "KeyInputManager.h"
#include "stdc++.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <random>





//****************************************************************************************************************************************************************
//IMPORTANT PART FOR GUI IMPLEMENTATION
//Below this comment, I have declared an atomic global variable for each changeable synth parameter. I want you to map them to a knobs, sliders, buttons, etc.
// 
// Note: Atomic variables are special variables designed for multithreading. They're designed to prevent race conditions, usually caused by multiple threads/cores
// accessing the same variable at once. Any variables that can be accessed by both the GUI and the audio code should be declared as atomics.
// 
// Note #2: It may be necessary to implement something called cache padding so each variable is stored on a separate cache. This is recommended for optimization,
// but I left it out for the sake of simplicity and readability. I may change my mind on this later.
//****************************************************************************************************************************************************************

//GENERAL SYNTH PARAMETERS
//These parameters require reinitialization of the audio device to be changed.
std::atomic<int> voices = 16;//This should be changeable via dropdown menu
std::atomic<int> sampRate = 44100;//This should be changeable via dropdown menu
std::atomic<int> bufferSize = 256;//This should be changeable via dropdown menu
int channelCount = 2;//I don't think this should be changeable. I'll decide later

std::atomic<bool> reInit = false; //Set this True if any of the above parameters are changed. This should trigger the audioStart() method and reinitialize the device.


//KEYBOARD INPUT VARIABLES (NONE OF THESE SHOULD BE CHANGEABLE DIRECTLY VIA THE GUI)
bool noteHeld[64]; //An array that says whether each note is currently being held down
std::vector<char> notesHeld; //A vector containing all notes currently being held in order of when they were pressed
int oscAmpGoal = 0;//Not changeable
char noteSet = ' ';//Not changeable



//OSCILLATOR PARAMETERS
std::atomic<int> oscType = 4;//This should be changeable via dropdown menu
std::atomic<float> oscAmp = 1.0;//This should be changeable via knob/slider
std::atomic<float> oscFreq = 220;//This is set by KeyInputManager.h
std::atomic<float> oscPhase = 0.0; //DON'T CHANGE THIS
std::atomic<float> oscPhaseOffset = 0.0; //This should be changeable


//ENVELOPE PARAMETERS
std::atomic<float> attack = 10;//This should be changeable (range from 0.01 to 20)
std::atomic<float> decay = 1;//This should be changeable (range from 0.01 to 20)
std::atomic<float> sustain = 1;//This should be changeable (range from 0 to 1)
std::atomic<float> release = 1;//This should be changeable (range from 0.01 to 20)
int adsrState = 0;//Not changeable
float oscAmpMultiplier = 0;//Not changeable


//FILTER PARAMETERS
std::atomic<float> cutoff = 440; //This should be changeable (range from 1 to 20,000)
std::atomic<float> q = 1; //This should be changeable (range from 0 to 10)
std::atomic<float> filterType = 1; //This should be changeable (range from -1 to 1)
std::atomic<int> filterOrder = 1; //This should be changeable via dropdown menu (range from 1 to 4)
float biqCoefs[5] = { 0,0,0,0,0 };//Not changeable
//float filterOutReg[channelCount][2] = { 0,0 };//Not changeable
//float filterInReg[channelCount][3] = { 0,0,0, };//Not changeable
std::vector<std::vector<float>> filterOutReg(channelCount, std::vector<float>(2, 0)); //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
std::vector<std::vector<float>> filterInReg(channelCount, std::vector<float>(3, 0));


// This loop is called by the audio device once per buffer. It generates audio data in batches and writes it to the buffer.
int audioLoop(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void* userData)
{
    double* buffer = (double*)outputBuffer; //I think this could be a float instead of a double. Will try that later.
    double* lastValues = (double*)userData;

    if (status)
        wxLogMessage("System Underflow Detected");
    keyInputManager(); //deals with the keyboard input
    noteSetter(); //sets the osc frequency based on the note selected

	oscPhase = oscillator(buffer, nBufferFrames, channelCount, oscFreq, oscType, oscAmp, oscPhase); //This is where the oscillator method is called.
    //The buffer is filled with the audio data generated by the oscillator. The phase is updated by the oscillator method and returned to the main loop.

    envelope(buffer, nBufferFrames, channelCount, sampRate, attack, decay, sustain, release, adsrState);

    biquadCoefs(sampRate, cutoff, q, filterType, biqCoefs); //Sets the coefficients used to calculate the filter output
    //filter(buffer, nBufferFrames, channelCount, sampRate, biqCoefs);

    return 0;
}

class AudioManager {
public:
    AudioManager() : running(false) {}

    void start() {//Starts the audio on a separate thread so 

        audioThread = std::thread(&AudioManager::audioStart, this);
        audioThread.detach();
    }

    void stop() {
        running = false;
        if (audioThread.joinable()) {
            audioThread.join();
        }
    }

private:
    void audioStart() { //Initializes the audio device.



        RtAudio dac;
        std::vector<unsigned int> deviceIds = dac.getDeviceIds();
        if (deviceIds.size() < 1) {
            wxLogMessage("No audio devices found!");
            return;
        }

        RtAudio::StreamParameters parameters;
		parameters.deviceId = dac.getDefaultOutputDevice(); //Set the device ID to the default output device
		parameters.nChannels = channelCount; //Set the number of channels to 2 (left and right channel for stereo)
        parameters.firstChannel = 0;
		unsigned int sampleRate = sampRate; // Standard CD quality sample rate
		unsigned int bufferFrames = bufferSize; // The number of sample frames. Make this higher for less CPU usage, or lower for less latency. This should be changable by the user in the final version. Also, it should be a power of two.
		double data[2] = { 0, 0 }; //IDK what this does but it was in the example code for the library i downloaded so I'm keeping it for now. I'll remove it if it turns out to be unnecessary.

        if (dac.openStream(&parameters, NULL, RTAUDIO_FLOAT64, sampleRate,
            &bufferFrames, &audioLoop, (void*)&data)) {
            return; // Checks for a problem with device settings
        }

        // Once the stream is opened, this starts the stream
        if (dac.startStream()) {
            return;
        }

        running = true;
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Stops the audio stream
        if (dac.isStreamRunning())
            dac.stopStream();

        if (dac.isStreamOpen()) dac.closeStream();

    }

	std::thread audioThread; //Create a new thread to run the audio independent of the GUI
	std::atomic<bool> running; //This value is used to stop the audio thread when the program closes. Since the stop() function is called by the App class destructor, it is declared as an atomic to avoic race conditions
};

enum IDs {
    BUTTON_ID = 2
};

class App : public wxApp {
public:


    bool OnInit() { //This is the entry point for the program.
        wxFrame* window = new wxFrame(NULL, wxID_ANY, "GUI Test", wxDefaultPosition, wxSize(600, 400));
        wxPanel* panel = new wxPanel(window);


        window->Show();

        audioManager.start();//Starts the audio in a separate thread

        return true;
    }

    int OnExit() {
        audioManager.stop();
        return 0;
    }

private:
    AudioManager audioManager;
};

wxIMPLEMENT_APP(App);