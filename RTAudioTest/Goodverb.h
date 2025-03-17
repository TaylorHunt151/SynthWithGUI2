#pragma once

#include <atomic>
#include <vector>
#include <cmath>

class Goodverb { //a reverb that (hopefully) doesn't sound like trash. Work in progress
	//NOTE: The reverb will be based on Dattorro's Plate Reverb, a very popular reverb algorithm designed in the 1997.
public:
	std::atomic<bool> on = true; //Turns delay on or off. Controlled via checkbox in GUI.
	std::atomic<double> delayTime = .03; //Delay time in seconds. Controlled via GUI. Should range from 0.001 to .005.
	std::atomic<double> wetMix = .25; //Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob/slider in GUI.
	std::atomic<double> distortion = .01;//adds distortion in the feedback loop (after the filter). Range from .01 to 10
	std::atomic<double> feedback = .92; //Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.

	std::vector<double> tap; //Vector that stores the delayed signal.
	int writeIndex = 0; //Index for writing to the delay buffer.

	std::atomic<double> hpCutoff = 5000; //This should be changeable via knob/slider (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> hpQ = 1; //This should be changeable (range from 0 to 10) via knob/slider
	std::atomic<int> filterOrder = 1; //This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> bqdOutReg0; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> bqdInReg0;


	Goodverb() { //Initialize variables to correct sizes
		tempBuffer.resize(bufferSize.load() * channelCount);
		bqdOutReg0.resize(channelCount, std::vector<double>(2, 0));
		bqdInReg0.resize(channelCount, std::vector<double>(3, 0));
	}


	void biquadCoefs(double* coefs, double cutoff, double q, double type) {//copied from VoiceFunctions.h. This can only create coefficients for low-pass or hi-pass, so I'll need another coefficient generator for the allpass filters

		double w = (2 * M_PI) * (cutoff / 44100); 
		double a = sin(w) / (2 * q);

		if (type <= 0) {
			coefs[0] = ((((-1) * type * (1 - cos(w)) / 2) + (1 - type * (-1)) * (a)) / 2) / (1 + a);
			coefs[1] = (1 - cos(w)) / (1 + a);
			coefs[2] = ((((-1) * type * (1 - cos(w)) / 2) + (1 - type * (-1)) * (0 - a)) / 2) / (1 + a);
			coefs[3] = (-2 * cos(w)) / (1 + a);
			coefs[4] = (1 - a) / (1 + a);
		}
		else {
			coefs[0] = (((type * (1 + cos(w)) / 2) + (1 - type) * (a)) / 2) / (1 + a);
			coefs[1] = (type * (-1 + cos(w))) / (1 + a);
			coefs[2] = (((type * (1 + cos(w)) / 2) + (1 - type) * (-a)) / 2) / (1 + a);
			coefs[3] = (-2 * cos(w)) / (1 + a);
			coefs[4] = (1 - a) / (1 + a);
		}

	}


	void allpassCoefs(double* coefs, double cutoff, double q) {//Creates coefficients to configure the biquad filter as an all-pass filter. All-pass filters are used to diffuse the sound in a reverb

		double w = (2 * M_PI) * (cutoff / 44100);
		double a = sin(w) / (2 * q);

		coefs[0] = (1 - a) / (1 + a);
		coefs[1] = (-2 * cos(w)) / (1 + a);
		coefs[2] = 1;
		coefs[3] = coefs[1];
		coefs[4] = coefs[0];

	}

	void biquadFilter(std::vector<std::vector<double>> filterInReg, std::vector<std::vector<double>> filterOutReg, double biqCoefs[], double *tempBuffer) {//copied from VoiceFunctions.h
		int index = 0;
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				index = i * channelCount + j;

				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[index];

				tempBuffer[index] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Filter math equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[index];
			}
		}

	}

	void reverb(double* buffer) {

		

	}


};