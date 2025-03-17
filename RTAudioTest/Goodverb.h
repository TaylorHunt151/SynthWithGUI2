#pragma once

#include <atomic>
#include <vector>
#include <cmath>

class Goodverb { //a reverb that (hopefully) doesn't sound like trash. Work in progress
public:
	std::atomic<bool> on = true; //Turns delay on or off. Controlled via checkbox in GUI.
	std::atomic<double> delayTime = .03; //Delay time in seconds. Controlled via GUI. Should range from 0.001 to .005.
	std::atomic<double> wetMix = .25; //Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob/slider in GUI.
	std::atomic<double> distortion = .01;//adds distortion in the feedback loop (after the filter). Range from .01 to 10
	std::atomic<double> feedback = .92; //Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.

	std::vector<double> tap; //Vector that stores the delayed signal.
	int writeIndex = 0; //Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 880; //This should be changeable via knob/slider (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable (range from 0 to 10) via knob/slider
	std::atomic<double> filterType = 0; //This should be changeable (range from -1 to 1) via knob/slider
	std::atomic<int> filterOrder = 1; //This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double cutoff = cutoffSet;
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;


	Reverb() { //Initialize variables to correct sizes
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
		int delaySamples = static_cast<int>(delayTime * 44100); //Converts delay time from seconds to samples.

		tempBuffer.resize(bufferSize.load() * channelCount); //Resizes tempBuffer to the correct size

		//Copy input buffer to tempBuffer
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				tempBuffer[i * channelCount + j] = buffer[i * channelCount + j];
			}
		}

		biquadCoefs();
		biquadFilter();

		if (tap.size() != delaySamples * channelCount) {
			tap.resize(delaySamples * channelCount, 0); //Resizes the tap vector to the correct size.
			writeIndex = 0; //Reset write index when delay time changes.
		}

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int readIndex = (writeIndex + tap.size() - delaySamples * channelCount) % tap.size(); //Calculate read index for circular buffer.
				double delayedSample = tap[readIndex + j]; //Get the delayed sample.


				tempBuffer[i * channelCount + j] *= distortion;
				tempBuffer[i * channelCount + j] = atan(tempBuffer[i * channelCount + j]);
				tempBuffer[i * channelCount + j] /= distortion;

				//Mix the delayed sample with the current sample.
				buffer[i * channelCount + j] = buffer[i * channelCount + j] * (1.0 - wetMix) + delayedSample * wetMix;

				//Write the current sample to the delay buffer with feedback.
				tap[writeIndex + j] = tempBuffer[i * channelCount + j] + delayedSample * feedback;
			}
			//Increment write index and wrap around if necessary.
			writeIndex = (writeIndex + channelCount) % tap.size();
		}
	}


};