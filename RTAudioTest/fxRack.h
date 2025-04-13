#pragma once

/*****************************************************************************
* NOTE: The FX Rack should be its own section of the GUI. It should be divided
* into sub-sections, with one sub-section for each class in this file. The
* only effect on the effect rack that isn't included in this file is 
* Goodverb, which is the reverb effect. This is because the reverb effect is
* very complicated and required a lot of code to work. Every single effect
* should have its own sub-section, including Goodverb. They should be appear
* from left to right in this order: Flanger, Chorus, Delay, Goodverb,
* Distortion. This is because this is the order in which the effects are
* applied to the signal.
* 
* Above each class, you will find a list of variables, including atomics.
* Each atomic variable should have its own UI element. There should be a 
* comment next to each atomic variable explaining what type of UI element I want
* you to add, what range of values it should have, etc. 
* 
* You may alter the attributes like so: "obj.attribute = value;", where obj
* is the name of the effect you wish to access, "attribute" is the name of
* the attribute you wish to alter, and "value" is the name of the value you wish
* to assign to said attribute.
* 
* Here is the name of the objects, as declared in main.cpp: Delay is dly,
* Distortion is distortion, Flanger is flanger, Chorus is chorus, and
* Goodverb is goodverb. These are the "obj" names you will use to access the
* attributes you wish to alter via the UI, as stated above.
* 
* If anything is unclear, please don't hesitate to reach out to me!
* 
* -Tay
******************************************************************************/

#include <atomic>
#include <vector>
#include <cmath>

//NOTE: The designs of all these effects are heavily influenced by the book Designing Audio Effect Plugins in C++ by Will C. Pirkle.

extern int channelCount;
extern std::atomic<int> bufferSize; //Dont add a UI element for this

class Delay {
public:
    std::atomic<bool> on = false; //Turns delay on or off. Controlled via checkbox in GUI.
    std::atomic<double> delayTime = .5; //Delay time in seconds. Controlled via GUI. Should range from 0.001 to 10. Exponential scale.
    std::atomic<double> wetMix = 0.5; //Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob in GUI. Linear scale.
	std::atomic<double> drive = .1;//Controls the distortion's harshness. Range from 0 to 1, controlled via knob, logarithmic scale.
	std::atomic<int> distType = 0; //Changeable via dropdown menu, range 0 to 5, controls distortion type.
	std::atomic<double> distMix = 0.25; //Changeable via knob, range 0 to 1, controls the amount of distortion. Linear scale.
    std::atomic<double> feedback = 0.5; //Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.5. Linear scale.

    std::vector<double> tap; //Vector that stores the delayed signal.
    int writeIndex = 0; //Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 440; //This should be changeable via knob (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable (range from 0.1 to 10) via knob/slider. Exponential scale.
	std::atomic<double> filterType = 0; //This should be changeable (range from -1 to 1) via knob/slider. Linear scale.
	//std::atomic<int> filterOrder = 1; //This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double cutoff = cutoffSet;
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;


    Delay() {//initialize variables to the correct sizes
        tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
		tap.resize(20 * 44100, 0);
    }

	double sign(double x) {
		if (x >= 0) { return 1; }
		else { return -1; }
	}
	void biquadCoefs() { //Finds the coefficients 
		//NOTE: biquad filters have a unique trait. They can be any type of filter depending on how you set your coefficients.
		//This means they can be used as lowpass, bandpass, or highpass filters. You can even blend them together to create hybrid filters, which I do below.
		cutoff = cutoffSet;
		double w = (2 * 3.14159) * (cutoff / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {//If filerType >= 0, I take the equation for a lowpass filter and a bandpass filter and "blend" them together using a weighted average. 
			double filType = filterType * -1;
			biqCoefs[0] = (((filType * (1 - cos(w)) / 2) + (1 - filType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = (((filType * (1 - cos(w)) / 2) + (1 - filType) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else { //If filterType < 0, I take the equation for a highpass filter and a bandpass filter and "blend" them together using a weighted average.
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}//This implementation allows the user too seamlessly blend between the three main filter types, giving extra control over the timbre of the synth.

		//gain compensation
		//Note: Biquad filters naturally will have their gain change depending on what frequency they are tuned to. Robert Bristow-Johnson's Audio EQ Cookbook includes some formulas to help us compensate for this and have more consistent gain across the frequency spectrum.
		double gain = 1;
		if (filterType >= 0.5) {//Formula for if the filter is lowpass
			gain = (biqCoefs[0] + biqCoefs[1] + biqCoefs[2]) / (1 + biqCoefs[3] + biqCoefs[4]);
		}
		else if (filterType <= -0.5) {//formula for if the filter is highpass
			gain = (biqCoefs[0] - biqCoefs[1] + biqCoefs[2]) / (1 - biqCoefs[3] + biqCoefs[4]);
		}
		else {//Formula for if the filter is bandpass
			double real = biqCoefs[0] + biqCoefs[1] * cos(w) + biqCoefs[2] * cos(2 * w);
			double imag = biqCoefs[1] * sin(w) + biqCoefs[2] * sin(2 * w);
			double mag = sqrt(real * real + imag * imag);
			gain = mag / sqrt(1 + biqCoefs[3] * biqCoefs[3] + biqCoefs[4] * biqCoefs[4] + 2 * biqCoefs[3] * (1 + biqCoefs[4]) * cos(w) + 2 * biqCoefs[4] * cos(2 * w));
		}

		if (gain != 0.0) {
			biqCoefs[0] /= gain;
			biqCoefs[1] /= gain;
			biqCoefs[2] /= gain;
		}

	}

	void biquadFilter() { //This is a standard biquad filter. 

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];//Setting the input registers

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Biquad filter equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j]; //setting the output registers
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
		if (on) {
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



			for (int i = 0; i < bufferSize; i++) {
				for (int j = 0; j < channelCount; j++) {
					int readIndex = (writeIndex + tap.size() - delaySamples * channelCount) % tap.size(); //Calculate read index for circular buffer.
					double delayedSample = tap[readIndex + j]; //Get the delayed sample.

					distort(delayedSample);

					buffer[i * channelCount + j] = buffer[i * channelCount + j] * (1.0 - wetMix) + delayedSample * wetMix;				//Mix the delayed sample with the current sample.

					tap[writeIndex + j] = tempBuffer[i * channelCount + j] + delayedSample * feedback;				//Write the current sample to the delay buffer with feedback.
				}
				writeIndex = (writeIndex + channelCount) % tap.size();			//Increment write index and wrap around if necessary.

			}
		}
	}


};

class Flanger {
public:
    std::atomic<bool> on = false; //Turns delay on or off. Controlled via checkbox in GUI.
    std::atomic<double> delayTime = .003; //Delay time in seconds. Controlled via knob in GUI. Should range from 0.0005 to .005. Linear scale.
    std::atomic<double> wetMix = 0.5; //Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob in GUI. Linear scale.
	std::atomic<double> drive = .1;//Controls the distortion's harshness. Range from 0 to 1, controlled via knob, logarithmic scale.
	std::atomic<double> distMix = 0.25; //Changeable via knob, range 0 to 1, controls the amount of distortion. Linear scale.
    std::atomic<double> feedback = 0.99; //Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.5. Linear scale.
	std::atomic<int> distType = 0; //Controls the distortion type. Controlled via dropdown menu, range from 0 to 5
	std::atomic<double> modAmnt = .002;
	std::atomic<double> modFreq = .1;

	int modAmp = 0;
	double modPhase = 0;
	std::vector<int> modBuff;
    std::vector<double> tap; //Vector that stores the delayed signal.
    int writeIndex = 0; //Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 440; //This should be changeable via knob (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable (range from 0.1 to 10) via knob/slider. Exponential scale.
	std::atomic<double> filterType = 0; //This should be changeable (range from -1 to 1) via knob/slider. Linear scale.
	//std::atomic<int> filterOrder = 1; //This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double cutoff = cutoffSet;
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;


    Flanger() {//initialize variables to the correct sizes
        tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
		modBuff.resize(channelCount * bufferSize, 0);
		tap.resize(.006 * 44100, 0);
    }

	double sign(double x) {
		if (x >= 0) { return 1; }
		else { return -1; }
	}
	void biquadCoefs() { //Finds the coefficients 
		//NOTE: biquad filters have a unique trait. They can be any type of filter depending on how you set your coefficients.
		//This means they can be used as lowpass, bandpass, or highpass filters. You can even blend them together to create hybrid filters, which I do below.
		cutoff = cutoffSet;
		double w = (2 * 3.14159) * (cutoff / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {//If filerType >= 0, I take the equation for a lowpass filter and a bandpass filter and "blend" them together using a weighted average. 
			double filType = filterType * -1;
			biqCoefs[0] = (((filType * (1 - cos(w)) / 2) + (1 - filType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = (((filType * (1 - cos(w)) / 2) + (1 - filType) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else { //If filterType < 0, I take the equation for a highpass filter and a bandpass filter and "blend" them together using a weighted average.
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}//This implementation allows the user too seamlessly blend between the three main filter types, giving extra control over the timbre of the synth.

		//gain compensation
		//Note: Biquad filters naturally will have their gain change depending on what frequency they are tuned to. Robert Bristow-Johnson's Audio EQ Cookbook includes some formulas to help us compensate for this and have more consistent gain across the frequency spectrum.
		double gain = 1;
		if (filterType >= 0.5) {//Formula for if the filter is lowpass
			gain = (biqCoefs[0] + biqCoefs[1] + biqCoefs[2]) / (1 + biqCoefs[3] + biqCoefs[4]);
		}
		else if (filterType <= -0.5) {//formula for if the filter is highpass
			gain = (biqCoefs[0] - biqCoefs[1] + biqCoefs[2]) / (1 - biqCoefs[3] + biqCoefs[4]);
		}
		else {//Formula for if the filter is bandpass
			double real = biqCoefs[0] + biqCoefs[1] * cos(w) + biqCoefs[2] * cos(2 * w);
			double imag = biqCoefs[1] * sin(w) + biqCoefs[2] * sin(2 * w);
			double mag = sqrt(real * real + imag * imag);
			gain = mag / sqrt(1 + biqCoefs[3] * biqCoefs[3] + biqCoefs[4] * biqCoefs[4] + 2 * biqCoefs[3] * (1 + biqCoefs[4]) * cos(w) + 2 * biqCoefs[4] * cos(2 * w));
		}

		if (gain != 0.0) {
			biqCoefs[0] /= gain;
			biqCoefs[1] /= gain;
			biqCoefs[2] /= gain;
		}

	}

	void biquadFilter() { //This is a standard biquad filter. 

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];//Setting the input registers

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Biquad filter equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j]; //setting the output registers
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

	void modulator() {
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				modBuff[i * channelCount + j] = floor(modAmp * (2 * abs(2 * modPhase - 1) - 1)); //Calculates triangle wave values
				modPhase += modFreq / 44100.0; //Keeps track of the phase
				if (modPhase >= 1.0) {
					modPhase -= 1; //This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
				}
			}
		}
	}

	void flanger(double* buffer) {
		if (on) {
			int delaySamples = static_cast<int>(delayTime * 44100); //Converts delay time from seconds to samples.
			int modSamples = static_cast<int>(modAmnt * 44100);//converts mod depth from seconds to samples.
			modAmp = static_cast<int>(modAmnt * 44100);

			modBuff.resize(bufferSize.load() * channelCount, 0);

			tempBuffer.resize(bufferSize.load() * channelCount); //Resizes tempBuffer to the correct size

			//Copy input buffer to tempBuffer
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
					int readIndex = (writeIndex + tap.size() - delaySamples * channelCount + modBuff[channelCount * i + j]) % tap.size(); //Calculate read index for circular buffer.

					if (readIndex < 0) {
						readIndex = tap.size() + readIndex;
					}

					if (readIndex > tap.size()  - channelCount) {
						readIndex = tap.size() - channelCount;
					}
					
					double delayedSample = tap[readIndex + j]; //Get the delayed sample.
					//distort(delayedSample);

					buffer[i * channelCount + j] = buffer[i * channelCount + j] * (1.0 - wetMix) + delayedSample * wetMix;				//Mix the delayed sample with the current sample.

					tap[writeIndex + j] = tempBuffer[i * channelCount + j] + delayedSample * feedback;				//Write the current sample to the delay buffer with feedback.
				}
				writeIndex = (writeIndex + channelCount) % tap.size();			//Increment write index and wrap around if necessary.

			}
		}
	}


};


class Distortion{
public:
	std::atomic<bool> on = false; //Turns distortion on or off. Controlled via checkbox in GUI.
	std::atomic<double> wetMix = .125; //Controls how loud the distorted signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob in GUI. Linear scale
	std::atomic<double> drive = .1;//Drives the signal into the distortion algorithm. Ranges from 0.01 to 1. Controlled via knob/slider in GUI. Logarithmic scale.
	std::atomic<int> type = 1; //Ranges from 0 to 3 (or more, will decide later). Controlled via dropdown menu in GUI.

	std::vector<double> tap; //Vector that stores the delayed signal.
	int writeIndex = 0; //Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 440; //This should be changeable via knob (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable (range from 0.1 to 10) via knob. Exponential scale.
	std::atomic<double> filterType = 0; //This should be changeable (range from -1 to 1) via knob. Linear scale.


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg; //Creates a 2d vector for the filter's input registers

	double cutoff = cutoffSet;

	Distortion() { //Initializes distortion attributes to correct sizes
		tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
	}



	void biquadCoefs() { //Finds the coefficients 
		//NOTE: biquad filters have a unique trait. They can be any type of filter depending on how you set your coefficients.
		//This means they can be used as lowpass, bandpass, or highpass filters. You can even blend them together to create hybrid filters, which I do below.
		//cutoff = cutoffSet;
		double w = (2 * 3.14159) * (cutoff / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {//If filerType >= 0, I take the equation for a lowpass filter and a bandpass filter and "blend" them together using a weighted average. 
			double filType = filterType * -1;
			biqCoefs[0] = (((filType * (1 - cos(w)) / 2) + (1 - filType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = (((filType * (1 - cos(w)) / 2) + (1 - filType) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else { //If filterType < 0, I take the equation for a highpass filter and a bandpass filter and "blend" them together using a weighted average.
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}//This implementation allows the user too seamlessly blend between the three main filter types, giving extra control over the timbre of the synth.

		//gain compensation
		//Note: Biquad filters naturally will have their gain change depending on what frequency they are tuned to. Robert Bristow-Johnson's Audio EQ Cookbook includes some formulas to help us compensate for this and have more consistent gain across the frequency spectrum.
		double gain = 1;
		if (filterType >= 0.5) {//Formula for if the filter is lowpass
			gain = (biqCoefs[0] + biqCoefs[1] + biqCoefs[2]) / (1 + biqCoefs[3] + biqCoefs[4]);
		}
		else if (filterType <= -0.5) {//formula for if the filter is highpass
			gain = (biqCoefs[0] - biqCoefs[1] + biqCoefs[2]) / (1 - biqCoefs[3] + biqCoefs[4]);
		}
		else {//Formula for if the filter is bandpass
			double real = biqCoefs[0] + biqCoefs[1] * cos(w) + biqCoefs[2] * cos(2 * w);
			double imag = biqCoefs[1] * sin(w) + biqCoefs[2] * sin(2 * w);
			double mag = sqrt(real * real + imag * imag);
			gain = mag / sqrt(1 + biqCoefs[3] * biqCoefs[3] + biqCoefs[4] * biqCoefs[4] + 2 * biqCoefs[3] * (1 + biqCoefs[4]) * cos(w) + 2 * biqCoefs[4] * cos(2 * w));
		}

		if (gain != 0.0) {
			biqCoefs[0] /= gain;
			biqCoefs[1] /= gain;
			biqCoefs[2] /= gain;
		}

	}

	void biquadFilter() { //This is a standard biquad filter. 

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];//Setting the input registers

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Biquad filter equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j]; //setting the output registers
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
		if (on) {
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


	}

};


class Chorus {
public:
	std::atomic<bool> on = false; //Turns delay on or off. Controlled via checkbox in GUI.
	std::atomic<double> delayTime = .03; //Delay time in seconds. Controlled via knob in GUI. Should range from 0.003 to .05. Linear scale.
	std::atomic<double> wetMix = 0.5; //Controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob in GUI. Linear scale.
	std::atomic<double> drive = .3;//Controls the distortion's harshness. Range from 0 to 1, controlled via knob, logarithmic scale.
	std::atomic<double> distMix = 0.25; //Changeable via knob, range 0 to 1, controls the amount of distortion. Linear scale.
	std::atomic<double> feedback = 0.5; //Controls delay feedback amount. Controlled via knob/slider in GUI. Range from 0 to 1.5. Linear scale.
	std::atomic<int> distType = 0; //Controls the distortion type. Controlled via dropdown menu, range from 0 to 5
	std::atomic<double> modAmnt = .02; //controlled via knob. Range from 0 to 0.05, linear scale
	std::atomic<double> modFreq = .1; //controlled via knob. Range from 0.01 to 10. Exponential scale.

	int modAmp = 0;
	double modPhase = 0;
	std::vector<int> modBuff;
	std::vector<double> tap; //Vector that stores the delayed signal.
	int writeIndex = 0; //Index for writing to the delay buffer.

	std::atomic<double> cutoffSet = 440; //This should be changeable via knob (range from 1 to 20,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable (range from 0.1 to 10) via knob/slider. Exponential scale.
	std::atomic<double> filterType = 0; //This should be changeable (range from -1 to 1) via knob/slider. Linear scale.
	//std::atomic<int> filterOrder = 1; //This should be changeable via dropdown menu (range from 1 to 4). It doesn't do anything yet


	std::vector<double> tempBuffer; //Temporary buffer to prevent the original signal from being altered
	double cutoff = cutoffSet;
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable

	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;


	Chorus() {//initialize variables to the correct sizes
		tempBuffer.resize(bufferSize.load() * channelCount);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
		filterInReg.resize(channelCount, std::vector<double>(3, 0));
		modBuff.resize(channelCount * bufferSize, 0);
		tap.resize(.1 * 44100, 0);
	}

	double sign(double x) {
		if (x >= 0) { return 1; }
		else { return -1; }
	}
	void biquadCoefs() { //Finds the coefficients 
		//NOTE: biquad filters have a unique trait. They can be any type of filter depending on how you set your coefficients.
		//This means they can be used as lowpass, bandpass, or highpass filters. You can even blend them together to create hybrid filters, which I do below.
		cutoff = cutoffSet;
		double w = (2 * 3.14159) * (cutoff / 44100);
		double a = sin(w) / (2 * q);

		if (filterType <= 0) {//If filerType >= 0, I take the equation for a lowpass filter and a bandpass filter and "blend" them together using a weighted average. 
			double filType = filterType * -1;
			biqCoefs[0] = (((filType * (1 - cos(w)) / 2) + (1 - filType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (1 - cos(w)) / (1 + a);
			biqCoefs[2] = (((filType * (1 - cos(w)) / 2) + (1 - filType) * (0 - a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else { //If filterType < 0, I take the equation for a highpass filter and a bandpass filter and "blend" them together using a weighted average.
			biqCoefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
			biqCoefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
			biqCoefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
			biqCoefs[3] = (-2 * cos(w)) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}//This implementation allows the user too seamlessly blend between the three main filter types, giving extra control over the timbre of the synth.

		//gain compensation
		//Note: Biquad filters naturally will have their gain change depending on what frequency they are tuned to. Robert Bristow-Johnson's Audio EQ Cookbook includes some formulas to help us compensate for this and have more consistent gain across the frequency spectrum.
		double gain = 1;
		if (filterType >= 0.5) {//Formula for if the filter is lowpass
			gain = (biqCoefs[0] + biqCoefs[1] + biqCoefs[2]) / (1 + biqCoefs[3] + biqCoefs[4]);
		}
		else if (filterType <= -0.5) {//formula for if the filter is highpass
			gain = (biqCoefs[0] - biqCoefs[1] + biqCoefs[2]) / (1 - biqCoefs[3] + biqCoefs[4]);
		}
		else {//Formula for if the filter is bandpass
			double real = biqCoefs[0] + biqCoefs[1] * cos(w) + biqCoefs[2] * cos(2 * w);
			double imag = biqCoefs[1] * sin(w) + biqCoefs[2] * sin(2 * w);
			double mag = sqrt(real * real + imag * imag);
			gain = mag / sqrt(1 + biqCoefs[3] * biqCoefs[3] + biqCoefs[4] * biqCoefs[4] + 2 * biqCoefs[3] * (1 + biqCoefs[4]) * cos(w) + 2 * biqCoefs[4] * cos(2 * w));
		}

		if (gain != 0.0) {
			biqCoefs[0] /= gain;
			biqCoefs[1] /= gain;
			biqCoefs[2] /= gain;
		}

	}

	void biquadFilter() { //This is a standard biquad filter. 

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				filterInReg[j][2] = filterInReg[j][1];
				filterInReg[j][1] = filterInReg[j][0];
				filterInReg[j][0] = tempBuffer[i * channelCount + j];//Setting the input registers

				tempBuffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Biquad filter equation

				filterOutReg[j][1] = filterOutReg[j][0];
				filterOutReg[j][0] = tempBuffer[i * channelCount + j]; //setting the output registers
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

	void modulator() {
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				modBuff[i * channelCount + j] = floor(modAmp * (2 * abs(2 * modPhase - 1) - 1)); //Calculates triangle wave values
				modPhase += modFreq / 44100.0; //Keeps track of the phase
				if (modPhase >= 1.0) {
					modPhase -= 1; //This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
				}
			}
		}
	}

	void chorus(double* buffer) {
		if (on) {
			int delaySamples = static_cast<int>(delayTime * 44100); //Converts delay time from seconds to samples.
			int modSamples = static_cast<int>(modAmnt * 44100);//converts mod depth from seconds to samples.
			modAmp = static_cast<int>(modAmnt * 44100);

			modBuff.resize(bufferSize.load() * channelCount, 0);

			tempBuffer.resize(bufferSize.load() * channelCount); //Resizes tempBuffer to the correct size

			//Copy input buffer to tempBuffer
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
					int readIndex = (writeIndex + tap.size() - delaySamples * channelCount + modBuff[channelCount * i + j]) % tap.size(); //Calculate read index for circular buffer.

					if (readIndex < 0) {
						readIndex = tap.size() + readIndex;
					}

					if (readIndex > tap.size() - channelCount) {
						readIndex = tap.size() - channelCount;
					}

					double delayedSample = tap[readIndex + j]; //Get the delayed sample.
					//distort(delayedSample);

					buffer[i * channelCount + j] = buffer[i * channelCount + j] * (1.0 - wetMix) + delayedSample * wetMix;				//Mix the delayed sample with the current sample.

					tap[writeIndex + j] = tempBuffer[i * channelCount + j] + delayedSample * feedback;				//Write the current sample to the delay buffer with feedback.
				}
				writeIndex = (writeIndex + channelCount) % tap.size();			//Increment write index and wrap around if necessary.

			}
		}
	}


};
