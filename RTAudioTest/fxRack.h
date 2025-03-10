#pragma once

#include <atomic>
#include <vector>
#include <queue>
#include <cmath>

// NOTE: The designs of all these effects are heavily influenced by the book Designing Audio Effect Plugins in C++ by Will C. Pirkle.

extern int channelCount;
extern std::atomic<int> bufferSize;

class Delay {
public:
    std::atomic<bool> on = true; // Turns delay on or off. Controlled via checkbox in GUI.
    std::atomic<double> delayTime = .5; // Delay time in seconds. Controlled via GUI. Should range from 0.001 to 10.
    std::atomic<double> wetMix = 0.5; // Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob/slider in GUI.
	std::atomic<double> drive = .1;//Controls the distortion's harshness. Range from 0 to 1
	std::atomic<int> distType = 0; //Changeable via dropdown menu, range 0 to 5, controls distortion type
	std::atomic<double> distMix = 0.25; //Changeable via knob, range 0 to 1, controls the amount of distortion.
    std::atomic<double> feedback = 0.3; // Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.5.

    std::vector<double> tap; // Vector that stores the delayed signal.
    int writeIndex = 0; // Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 220; //This should be changeable via knob/slider (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable (range from 0 to 10) via knob/slider
	std::atomic<double> filterType = 0; //This should be changeable (range from -1 to 1) via knob/slider
	std::atomic<int> filterOrder = 1; //This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double cutoff = cutoffSet;
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;


    Delay() {//initialize variables
        tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
		tap.resize(20 * 44100, 0);
    }

	double sign(double x) {
		if (x >= 0) { return 1; }
		else { return -1; }
	}
	void biquadCoefs() { //copied from VoiceFunctions

		double w = (2 * 3.14159) * (cutoff / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {
			biqCoefs[0] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else {
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}


	}

	void biquadFilter() { //copied from voicefunctions

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Filter math equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j];
			}
		}

	}
	double distort(double x) {

		double y = 0; 

		switch (distType) {//Decides which type of distortion to use. All distortion algorithms are taken from page 548 of Will Pirkle's aforementioned book.

			case 0: y = atan(drive * x) / atan(drive); break;//Arctan distortion

			case 1: y = sign(x) * (1 - exp(abs(drive * x))) / (1 - exp(-drive)); break; //Exponential fuzz

			case 2: y = tanh(x * drive) / tanh(drive); break; //Hyperbolic tangent

			case 3: y = 2 * (1 / (1 + exp(-drive * x))) - 1; break; //sigmoid

			case 4: y = (3 * x / 2) * (1 - (x * x / 3)); break; //Sigmoid2 (mild)

			case 5: y = x * x * x; break; //cubic distortion

		}
		return y;

	}
	void delay(double* buffer) {
		int delaySamples = static_cast<int>(delayTime * 44100); // Converts delay time from seconds to samples.

		tempBuffer.resize(bufferSize.load() * channelCount); // Resizes tempBuffer to the correct size

		// Copy input buffer to tempBuffer
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				tempBuffer[i * channelCount + j] = buffer[i * channelCount + j];
			}
		}

		biquadCoefs();
		biquadFilter();



		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int readIndex = (writeIndex + tap.size() - delaySamples * channelCount) % tap.size(); // Calculate read index for circular buffer.
				double delayedSample = tap[readIndex + j]; // Get the delayed sample.

				distort(delayedSample);

				buffer[i * channelCount + j] = buffer[i * channelCount + j] * (1.0 - wetMix) + delayedSample * wetMix;				// Mix the delayed sample with the current sample.

				tap[writeIndex + j] = tempBuffer[i * channelCount + j] + delayedSample * feedback;				// Write the current sample to the delay buffer with feedback.
			}
			writeIndex = (writeIndex + channelCount) % tap.size();			// Increment write index and wrap around if necessary.

		}
	}


};


class Reverb { //this reverb sounds terrible
public:
	std::atomic<bool> on = true; // Turns delay on or off. Controlled via checkbox in GUI.
	std::atomic<double> delayTime = .03; // Delay time in seconds. Controlled via GUI. Should range from 0.001 to .005.
	std::atomic<double> wetMix = .25; // Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob/slider in GUI.
	std::atomic<double> distortion = .01;//adds distortion in the feedback loop (after the filter). Range from .01 to 10
	std::atomic<double> feedback = .92; // Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.

	std::vector<double> tap; // Vector that stores the delayed signal.
	int writeIndex = 0; // Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 880; //This should be changeable via knob/slider (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable (range from 0 to 10) via knob/slider
	std::atomic<double> filterType = 0; //This should be changeable (range from -1 to 1) via knob/slider
	std::atomic<int> filterOrder = 1; //This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double cutoff = cutoffSet;
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;


	Reverb() {
		tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
	}



	void biquadCoefs() {//copied from VoiceFunctions.h

		double w = (2 * 3.14159) * (cutoff / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {
			biqCoefs[0] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else {
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}


	}

	void biquadFilter() {//copied from VoiceFunctions.h

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Filter math equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j];
			}
		}

	}



	void reverb(double* buffer) {
		int delaySamples = static_cast<int>(delayTime * 44100); // Converts delay time from seconds to samples.

		tempBuffer.resize(bufferSize.load() * channelCount); // Resizes tempBuffer to the correct size

		// Copy input buffer to tempBuffer
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				tempBuffer[i * channelCount + j] = buffer[i * channelCount + j];
			}
		}

		biquadCoefs();
		biquadFilter();

		if (tap.size() != delaySamples * channelCount) {
			tap.resize(delaySamples * channelCount, 0); // Resizes the tap vector to the correct size.
			writeIndex = 0; // Reset write index when delay time changes.
		}

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int readIndex = (writeIndex + tap.size() - delaySamples * channelCount) % tap.size(); // Calculate read index for circular buffer.
				double delayedSample = tap[readIndex + j]; // Get the delayed sample.


				tempBuffer[i * channelCount + j] *= distortion;
				tempBuffer[i * channelCount + j] = atan(tempBuffer[i * channelCount + j]);
				tempBuffer[i * channelCount + j] /= distortion;

				// Mix the delayed sample with the current sample.
				buffer[i * channelCount + j] = buffer[i * channelCount + j] * (1.0 - wetMix) + delayedSample * wetMix;

				// Write the current sample to the delay buffer with feedback.
				tap[writeIndex + j] = tempBuffer[i * channelCount + j] + delayedSample * feedback;
			}
			// Increment write index and wrap around if necessary.
			writeIndex = (writeIndex + channelCount) % tap.size();
		}
	}


};

class Distortion{
public:
	std::atomic<bool> on = true; // Turns delay on or off. Controlled via checkbox in GUI.
	std::atomic<double> wetMix = .125; // Controls how loud the distorted signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob/slider in GUI.
	std::atomic<double> drive = .1;//Drives the distortion into the distortion algorithm. Ranges from 0.01 to 1. Controlled via knob/slider in GUI
	std::atomic<int> type = 1; // Ranges from 0 to 3 (or more, will decide later). Controlled via dropdown menu in GUI.

	std::vector<double> tap; // Vector that stores the delayed signal.
	int writeIndex = 0; // Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 440; //This should be changeable via knob/slider (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable (range from 0 to 10) via knob/slider
	std::atomic<double> filterType = 0; //This should be changeable (range from -1 to 1) via knob/slider


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;


	Distortion() {
		tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
	}



	void biquadCoefs() { //copied from VoiceFunctions.h

		double w = (2 * 3.14159) * (cutoffSet / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {
			biqCoefs[0] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else {
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}


	}

	void biquadFilter() {//copied from VoiceFunctions.h

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Filter math equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j];
			}
		}

	}

	double sign(double x) {
		if (x >= 0) { return 1; }
		else { return -1; }
	}

	void distAlgorithms(int i){
		
		double x = tempBuffer[i]; //

		switch (type) {//Decides which type of distortion to use. All distortion algorithms are taken from page 548 of Will Pirkle's aforementioned book.

			case 0: tempBuffer[i] = atan(drive * x)/atan(drive); break;//Arctan distortion

			case 1: tempBuffer[i] = sign(x) * (1 - exp(abs(drive * x))) / (1 - exp(-drive)); break; //Exponential fuzz

			case 2: tempBuffer[i] = tanh(x * drive) / tanh(drive); break; //Hyperbolic tangent

			case 3: tempBuffer[i] = 2 * (1 / (1 + exp(-drive * x))) - 1; break; //sigmoid

			case 4: tempBuffer[i] = (3 * x / 2) * (1 - (x * x / 3)); break; //Sigmoid2 (mild)

			case 5: tempBuffer[i] = x * x * x; break; //cubic distortion

		}
		
	}

	void distort(double* buffer) {
		for (int i = 0; i < (bufferSize * channelCount); i++) {
			tempBuffer[i] = buffer[i];
		}
		biquadCoefs(); 
		biquadFilter();//Filtering the signal before distorting is is a common technique to shape the timbre of the distortion

		for (int i = 0; i < (bufferSize * channelCount); i++) {
			//tempBuffer[i] *= drive;
			distAlgorithms(i);
			if (drive > 1) {
				tempBuffer[i] /= drive * drive;
			}
			buffer[i] = tempBuffer[i] * wetMix + buffer[i] * (1 - wetMix);
		}




	}

};

class Flanger { //NOTE: A flanger is basically a delay with an LFO modulating the delay time. Also the delay time ranges from .5ms to 5ms
public:
	std::atomic<bool> on = true; // Turns flanger on or off. Controlled via checkbox in GUI.
	std::atomic<double> delayTime = 0.003; // Delay time in seconds. Controlled via GUI. Should range from 0.0005 to .005.
	std::atomic<double> wetMix = 0.125; // Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob/slider in GUI.
	std::atomic<double> modSpeed = 0.47; // Modulation speed in Hz. Range from 0.1 to 10. Controlled via knob/slider.
	std::atomic<double> modDepth = 0.0005; // Modulation depth. Range from 0.0001 to 0.003
	std::atomic<double> feedback = 0.25; // Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.

	std::vector<double> tap; // Vector that stores the delayed signal.
	int writeIndex = 0; // Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 440; // This should be changeable via knob/slider (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; // This should be changeable (range from 0 to 10) via knob/slider
	std::atomic<double> filterType = -0.5; // This should be changeable (range from -1 to 1) via knob/slider
	std::atomic<int> filterOrder = 1; // This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet

	std::vector<double> tempBuffer; // Temporary buffer to prevent the original signal from being altered
	std::vector<double> modBuffer; // Buffer for the modulation signal
	double modPhase = 0.3;
	double biqCoefs[5] = { 0,0,0,0,0 }; // Not changeable

	std::vector<std::vector<double>> filterOutReg; // Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;

	Flanger() { // Initialize variables
		tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
		tap.resize(20 * 44100, 0);
		modBuffer.resize(bufferSize.load() * channelCount);
	}

	double sign(double x) {
		return (x >= 0) ? 1 : -1;
	}

	void biquadCoefs() { // Copied from VoiceFunctions
		double w = (2 * 3.14159) * (cutoffSet / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {
			biqCoefs[0] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else {
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
	}

	void biquadFilter() { // Copied from VoiceFunctions
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; // Filter math equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j];
			}
		}
	}

	void modulator() {
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				modBuffer[i * channelCount + j] = modDepth * (2 * abs(2 * modPhase - 1) - 1); // Calculates triangle wave values
				modPhase += modSpeed / 44100.0; // Keeps track of the phase
				if (modPhase >= 1.0) {
					modPhase = 0;
				}// Wrap around phase
			}
		}
	}

	void flanger(double* buffer) {
		int baseDelaySamples = static_cast<int>(delayTime * 44100); // Converts delay time from seconds to samples

		tempBuffer.resize(bufferSize.load() * channelCount); // Resizes tempBuffer to the correct size

		// Copy input buffer to tempBuffer
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				tempBuffer[i * channelCount + j] = buffer[i * channelCount + j];
			}
		}

		biquadCoefs();
		biquadFilter();
		modulator();

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int modulatedDelaySamples = baseDelaySamples + static_cast<int>(modBuffer[i * channelCount + j] * 44100); // Modulate delay time

				int readIndex = (writeIndex + tap.size() - modulatedDelaySamples * channelCount) % tap.size(); // Calculate read index for circular buffer
				double delayedSample = tap[readIndex + j]; // Get the delayed sample

				buffer[i * channelCount + j] = buffer[i * channelCount + j] * (1.0 - wetMix) + delayedSample * wetMix; // Mix the delayed sample with the current sample

				tap[writeIndex + j] = tempBuffer[i * channelCount + j] + delayedSample * feedback; // Write the current sample to the delay buffer with feedback
			}
			writeIndex = (writeIndex + channelCount) % tap.size(); // Increment write index and wrap around if necessary
		}
	}
};


class Chorus { //NOTE: A Chorus is basically a flanger but with a slower delay time, ranging from 5 to 50 ms.
public:
	std::atomic<bool> on = true; // Turns flanger on or off. Controlled via checkbox in GUI.
	std::atomic<double> delayTime = 0.03; // Delay time in seconds. Controlled via GUI. Should range from 0.0005 to .005.
	std::atomic<double> wetMix = 0.125; // Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob/slider in GUI.
	std::atomic<double> modSpeed = 0.52; // Modulation speed in Hz. Range from 0.1 to 10. Controlled via knob/slider.
	std::atomic<double> modDepth = 0.005; // Modulation depth. Range from 0.001 to 0.03
	std::atomic<double> feedback = 0.75; // Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.

	std::vector<double> tap; // Vector that stores the delayed signal.
	int writeIndex = 0; // Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 440; // This should be changeable via knob/slider (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; // This should be changeable (range from 0 to 10) via knob/slider
	std::atomic<double> filterType = -0.5; // This should be changeable (range from -1 to 1) via knob/slider
	std::atomic<int> filterOrder = 1; // This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet

	std::vector<double> tempBuffer; // Temporary buffer to prevent the original signal from being altered
	std::vector<double> modBuffer; // Buffer for the modulation signal
	double modPhase = 0.3;
	double biqCoefs[5] = { 0,0,0,0,0 }; // Not changeable

	std::vector<std::vector<double>> filterOutReg; // Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;

	Chorus() { // Initialize variables
		tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
		tap.resize(20 * 44100, 0);
		modBuffer.resize(bufferSize.load() * channelCount);
	}

	double sign(double x) {
		return (x >= 0) ? 1 : -1;
	}

	void biquadCoefs() { // Copied from VoiceFunctions
		double w = (2 * 3.14159) * (cutoffSet / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {
			biqCoefs[0] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else {
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
	}

	void biquadFilter() { // Copied from VoiceFunctions
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; // Filter math equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j];
			}
		}
	}

	void modulator() {
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				modBuffer[i * channelCount + j] = modDepth * (2 * abs(2 * modPhase - 1) - 1); // Calculates triangle wave values
				modPhase += modSpeed / 44100.0; // Keeps track of the phase
				if (modPhase >= 1.0) {
					modPhase = 0;
				}// Wrap around phase
			}
		}
	}

	void chorus(double* buffer) {
		int baseDelaySamples = static_cast<int>(delayTime * 44100); // Converts delay time from seconds to samples

		tempBuffer.resize(bufferSize.load() * channelCount); // Resizes tempBuffer to the correct size

		// Copy input buffer to tempBuffer
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				tempBuffer[i * channelCount + j] = buffer[i * channelCount + j];
			}
		}

		biquadCoefs();
		biquadFilter();
		modulator();

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int modulatedDelaySamples = baseDelaySamples + static_cast<int>(modBuffer[i * channelCount + j] * 44100); // Modulate delay time

				int readIndex = (writeIndex + tap.size() - modulatedDelaySamples * channelCount) % tap.size(); // Calculate read index for circular buffer
				double delayedSample = tap[readIndex + j]; // Get the delayed sample

				buffer[i * channelCount + j] = buffer[i * channelCount + j] * (1.0 - wetMix) + delayedSample * wetMix; // Mix the delayed sample with the current sample

				tap[writeIndex + j] = tempBuffer[i * channelCount + j] + delayedSample * feedback; // Write the current sample to the delay buffer with feedback
			}
			writeIndex = (writeIndex + channelCount) % tap.size(); // Increment write index and wrap around if necessary
		}
	}
};
