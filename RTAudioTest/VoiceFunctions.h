/************************************************************************************************************************
* NOTE FOR GUI:
* In this header file, there is only one class with many functions. This is different from the rest of the files. The 
* functions Oscillator, Oscillator2, Envelope, and biquadFilter should each have their own sections with labels. The only
* function that will not have its own section is the biquadCoefs function. Above each function, you will find a list of
* variables, including atomics. Each atomic variable should have its own UI element. There should be a comment next to
* each atomic variable explaining what type of UI element I want you to add, what range of values it should have, etc.
* 
* You can alter these attributes by accessing them like so: 
* "for (int i = 0; i < voiceCount; i++){
*	voices[i].attribute = value;
* }"
* Where i is the index of the voice object to be altered, attribute is the name of the attribute you wish to alter, and
* value is the value you wish to assign to said attribute. 
* 
* If anything is unclear, please don't hesitate to reach out to me!
* 
* -Tay
*************************************************************************************************************************/

#pragma once
#include <random>
#include "Modulators.h"
#include <cmath>


extern std::atomic<int> bufferSize; //DON'T ADD A UI ELEMENT FOR THIS

extern std::vector<std::vector<double>> filterOutReg;
extern std::vector<std::vector<double>> filterInReg;
extern int channelCount;

struct GUIControls;


extern LFO* LFOs;

//Initialize random generator
std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(-1, 1);

class voice {

public:

	//LOCAL BUFFER
	std::vector<double> localBuff; //local buffer for any given voice. All the voice buffers get added together in the end.
	std::vector<double> localBuff2;
	std::atomic<double> semitones = 0;

	//FILTER REGISTERS
	std::vector<std::vector<double>> filterOutReg; //Creates a 2d vector storing the filter's output registers for each channel. Not changeable.
	std::vector<std::vector<double>> filterInReg;

	voice() { //initializer. Resizes important vectors to the correct sizes
		localBuff.resize((bufferSize.load() * channelCount),0);
		localBuff2.resize((bufferSize.load() * channelCount), 0);
		filterOutReg.resize(channelCount, std::vector<double>(2, 0));
        filterInReg.resize(channelCount, std::vector<double>(3, 0));
	}

	//OSCILLATOR PARAMETERS
	std::atomic<int> oscType = 2;//This should be changeable via dropdown menu, range 0 to 4
	std::atomic<double> oscAmp = 0.01;//This should be changeable via knob/slider, range 0 to 0.1. Scaled logarithmically.
	std::atomic<double> oscPhaseOffset = 0.0; //This should be changeable via knob, range -1 to 1. Linear scale.
	std::atomic<double> oscPitchShift = -0.05; //Changeable, from -1 to 1 via knob. Linear scale.

	double oscFreq = 220;//Don't change this. This is set by KeyInputManager.h
	double oscPhase = 0.0; //DON'T CHANGE THIS

	void oscillator(int buffSize, int channels) {
		for (int i = 0; i < buffSize * channels; i++) {//clears the buffer before processing. Must be done even if the oscillator is turned off
			localBuff[i] = 0;
		}
		if (oscType != 5) {//If the oscillator is not turned off..
			switch (oscType) {
			case 0: //Sine wave
				for (int i = 0; i < buffSize; i++) {
					for (int j = 0; j < channels; j++)
					{
						if (LFOs[0].carrier == 1 && LFOs[1].carrier == 1) { //If both LFO's are modulating this...
							localBuff[i * channels + j] = oscAmp * sin(2 * M_PI * (oscFreq * pow(2, oscPitchShift / 12)) * fmod(oscPhase + oscPhaseOffset, 1)); //Calculates sine wave values
						}
						else if (LFOs[0].carrier == 1) {
							localBuff[i * channels + j] = oscAmp * sin(2 * M_PI * ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, LFOs[0].localBuff[i] / 12)) * fmod(oscPhase + oscPhaseOffset, 1)); //Calculates sine wave values
						}
						else if (LFOs[1].carrier == 1) {
							localBuff[i * channels + j] = oscAmp * sin(2 * M_PI * ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, LFOs[1].localBuff[i] / 12)) * fmod(oscPhase + oscPhaseOffset, 1)); //Calculates sine wave values

						}
						else {
							localBuff[i * channels + j] = oscAmp * sin(2 * M_PI * (oscFreq * pow(2, oscPitchShift / 12)) * fmod(oscPhase + oscPhaseOffset, 1)); //Calculates sine wave values

						}
						oscPhase += 1.0 / 44100.0; //Keeps track of the phase
						if (oscPhase >= 1.0) {
							oscPhase -= 1;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
						}
					}
				}
				break;
			case 1: //Square wave
				for (int i = 0; i < buffSize; i++) {
					for (int j = 0; j < channels; j++)
					{
						localBuff[i * channels + j] = oscAmp * (fmod(oscPhase + oscPhaseOffset, 1) < 0.5 ? 1 : -1); //Calculates square wave values



						if (LFOs[0].carrier == 1 && LFOs[1].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, (LFOs[0].localBuff[i] + LFOs[1].localBuff[i]) / 12)) / 44100.0; //Keeps track of the phase
						}
						else if (LFOs[0].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, LFOs[0].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
						}
						else if (LFOs[1].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, LFOs[1].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
						}
						else {
							oscPhase += (oscFreq * pow(2, oscPitchShift / 12)) / 44100.0; //Keeps track of the phase

						}

						if (oscPhase >= 1.0) {
							oscPhase -= 1;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
						}
					}
				}
				break;
			case 2: //Sawtooth wave
				for (int i = 0; i < buffSize; i++) {
					for (int j = 0; j < channels; j++)
					{
						localBuff[i * channels + j] = oscAmp * (2 * fmod(oscPhase + oscPhaseOffset , 1) - 1); //Calculates sawtooth wave values
						if (LFOs[0].carrier == 1 && LFOs[1].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, (LFOs[0].localBuff[i] + LFOs[1].localBuff[i]) / 12)) / 44100.0; //Keeps track of the phase
						}
						else if (LFOs[0].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, LFOs[0].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
						}
						else if (LFOs[1].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, LFOs[1].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
						}
						else {
							oscPhase += (oscFreq * pow(2, oscPitchShift / 12)) / 44100.0; //Keeps track of the phase

						}

						if (oscPhase >= 1.0) {
							oscPhase -= 1;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
						}
					}
				}
				break;
			case 3: //Triangle wave
				for (int i = 0; i < buffSize; i++) {
					for (int j = 0; j < channels; j++) {
						localBuff[i * channels + j] = oscAmp * (2 * abs(2 * fmod(oscPhase + oscPhaseOffset, 1) - 1) - 1); //Calculates triangle wave values
						if (LFOs[0].carrier == 1 && LFOs[1].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, (LFOs[0].localBuff[i] + LFOs[1].localBuff[i]) / 12)) / 44100.0; //Keeps track of the phase
						}
						else if (LFOs[0].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, LFOs[0].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
						}
						else if (LFOs[1].carrier == 1) {
							oscPhase += ((oscFreq * pow(2, oscPitchShift / 12)) * pow(2, LFOs[1].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
						}
						else {
							oscPhase += (oscFreq * pow(2, oscPitchShift / 12)) / 44100.0; //Keeps track of the phase

						}
						if (oscPhase >= 1.0) {
							oscPhase -= 1; //This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
						}
					}
				}
				break;
			case 4: //noise

				for (int i = 0; i < buffSize; i++) {
					for (int j = 0; j < channels; j++) {
						localBuff[i * channels + j] = oscAmp * (distribution(generator) * 2 - 1); //Generates random numbers between -1 and 1 to create noise.
						//Noise is useful for making percussive sounds and ambience.
					}
				}

			}
		}
	}

	std::atomic<int> osc2Type = 2;//This should be changeable via dropdown menu, range 0 to 4
	std::atomic<double> osc2Amp = 0.01;//This should be changeable via knob/slider, range 0 to 0.1. Scaled logarithmically.
	std::atomic<double> osc2PhaseOffset = 0.3; //This should be changeable via knob, range -1 to 1. Scaled linearly
	std::atomic<double> osc2PitchShift = .1; //Changeable, from -1 to 1 via knob. Scaled linearly.

	double osc2Phase = 0.0; //DON'T CHANGE THIS

	void oscillator2(int buffSize, int channels) {
		for (int i = 0; i < buffSize * channels; i++) {//Clearing the buffer. This must be done even if osc2 is deactivated.
			localBuff2[i] = 0;
		}

		switch (osc2Type) {
		case 0: //Sine wave
			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++)
				{
					if (LFOs[0].carrier == 2 && LFOs[1].carrier == 2) { //If both LFO's are modulating this...
						localBuff2[i * channels + j] = osc2Amp * sin(2 * M_PI * (oscFreq * pow(2, osc2PitchShift / 12)) * fmod(osc2Phase + osc2PhaseOffset, 1)); //Calculates sine wave values
					}
					else if (LFOs[0].carrier == 2) {
						localBuff2[i * channels + j] = osc2Amp * sin(2 * M_PI * ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, LFOs[0].localBuff[i] / 12)) * fmod(osc2Phase + osc2PhaseOffset, 1)); //Calculates sine wave values
					}
					else if (LFOs[1].carrier == 2) {
						localBuff2[i * channels + j] = osc2Amp * sin(2 * M_PI * ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, LFOs[1].localBuff[i] / 12)) * fmod(osc2Phase + osc2PhaseOffset, 1)); //Calculates sine wave values

					}
					else {
						localBuff2[i * channels + j] = osc2Amp * sin(2 * M_PI * (oscFreq * pow(2, osc2PitchShift / 12)) * fmod(osc2Phase + osc2PhaseOffset, 1)); //Calculates sine wave values

					}
					osc2Phase += 1.0 / 44100.0; //Keeps track of the phase
					if (osc2Phase >= 1.0) {
						osc2Phase -= 1;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 1: //Square wave
			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++)
				{
					localBuff2[i * channels + j] = osc2Amp * (fmod(osc2Phase + osc2PhaseOffset, 1) < 0.5 ? 1 : -1); //Calculates square wave values

					if (LFOs[0].carrier == 2 && LFOs[1].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, (LFOs[0].localBuff[i] + LFOs[1].localBuff[i]) / 12)) / 44100.0; //Keeps track of the phase
					}
					else if (LFOs[0].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, LFOs[0].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
					}
					else if (LFOs[1].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, LFOs[1].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
					}
					else {
						osc2Phase += (oscFreq * pow(2, osc2PitchShift / 12)) / 44100.0; //Keeps track of the phase

					}

					if (osc2Phase >= 1.0) {
						osc2Phase -= 1;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 2: //Sawtooth wave
			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++)
				{
					localBuff2[i * channels + j] = osc2Amp * (2 * fmod(osc2Phase + osc2PhaseOffset, 1) - 1); //Calculates sawtooth wave values
					if (LFOs[0].carrier == 2 && LFOs[1].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, (LFOs[0].localBuff[i] + LFOs[1].localBuff[i]) / 12)) / 44100.0; //Keeps track of the phase
					}
					else if (LFOs[0].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, LFOs[0].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
					}
					else if (LFOs[1].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, LFOs[1].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
					}
					else {
						osc2Phase += (oscFreq * pow(2, osc2PitchShift / 12)) / 44100.0; //Keeps track of the phase

					}

					if (osc2Phase >= 1.0) {
						osc2Phase -= 1;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 3: //Triangle wave
			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++) {
					localBuff2[i * channels + j] = osc2Amp * (2 * abs(2 * fmod(osc2Phase + osc2PhaseOffset, 1) - 1) - 1); //Calculates triangle wave values
					if (LFOs[0].carrier == 2 && LFOs[1].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, (LFOs[0].localBuff[i] + LFOs[1].localBuff[i]) / 12)) / 44100.0; //Keeps track of the phase
					}
					else if (LFOs[0].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, LFOs[0].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
					}
					else if (LFOs[1].carrier == 2) {
						osc2Phase += ((oscFreq * pow(2, osc2PitchShift / 12)) * pow(2, LFOs[1].localBuff[i] / 12)) / 44100.0; //Keeps track of the phase
					}
					else {
						osc2Phase += (oscFreq * pow(2, osc2PitchShift / 12)) / 44100.0; //Keeps track of the phase

					}
					if (osc2Phase >= 1.0) {
						osc2Phase -= 1; //This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 4: //noise

			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++) {
					localBuff2[i * channels + j] = osc2Amp * (distribution(generator) * 2 - 1); //Generates random numbers between -1 and 1 to create noise.
					//Noise is useful for making percussive sounds and ambience.
				}
			}
			break;
		case 5://silence
			for (int i = 0; i < buffSize * channels; i++) {
				localBuff2[i] = 0;
			}
			break;
		}
			

		for (int i = 0; i < buffSize * channels; i++) {

			localBuff[i] += localBuff2[i];
		}
		
	}


	//ENVELOPE PARAMETERS
	std::atomic<double> attack = .1;//This should be changeable via slider (range from 0.01 to 20). Exponential scale.
	std::atomic<double> decay = .1;//This should be changeable via slider (range from 0.01 to 20). Exponential scale.
	std::atomic<double> sustain = 1;//This should be changeable via slider (range from 0 to 1). Exponential scale.
	std::atomic<double> release = .1;//This should be changeable via slider (range from 0.01 to 20). Exponential scale.

	int oscAmpGoal = 0;//not changeable
	int adsrState = 0;//Not changeable
	double oscAmpMultiplier = 0;//Not changeable

	void envelope(int buffSize, int channels, int sampleRate) {
		double atk = attack.load();
		double dec = decay.load();
		double sus = sustain.load();
		double rel = release.load();
		atk *= sampleRate; //scaling adsr values to account for samplerate
		dec *= sampleRate;
		rel *= sampleRate;
		for (int i = 0; i < buffSize; i++) {
			for (int j = 0; j < channels; j++) {
				if (oscAmpGoal == 1) {
					if (adsrState == 0) { //attack stage
						if (oscAmpMultiplier < 1) {
							oscAmpMultiplier += 1 / atk;
						}
						else {
							adsrState = 1;
						}
					}
					if (adsrState == 1) { //decay stage
						if (oscAmpMultiplier > sus) {
							oscAmpMultiplier -= (1 - sus) / dec;
						}
						else {
							oscAmpMultiplier = sus;
							adsrState = 2;
						}
					}
					//if (adsrState == 2) { //sustain stage
					//	oscAmpMultiplier = sus;
					//}
				}
				else {
					if (adsrState != 3) {
						adsrState = 3; //release stage
					}
					if (oscAmpMultiplier > 0) {
						oscAmpMultiplier -= 1 / rel;
						if (oscAmpMultiplier < 0) {
							oscAmpMultiplier = 0;
						}
					}
				}
				localBuff[i * channels + j] *= oscAmpMultiplier;
			}
		}
	}

	//FILTER PARAMETERS
	std::atomic<bool> filtOn = false; //Controlled via checkbox in the GUI. Controls whether the filter is activated.
	std::atomic<double> cutoffSet = 880; //this should be changeable via knob (range from 80 to 18,000, default 220. Scaled exponentially)
	std::atomic<double> q = 1; //This should be changeable via knob. (range from 0.01 to 10). Logarithmic scale
	std::atomic<double> filterType = 1; //This should be changeable via knob. (range from -1 to 1), linear scale.
	std::atomic<bool> keyTrack = true; //This should be changeable via checkbox in the GUI.
	double cutoff = cutoffSet;
	double biqCoefs[5] = { 0,0,0,0,0 };//Not changeable


	void biquadCoefs(int sampleRate) { //Finds the coefficients 
		//NOTE: biquad filters have a unique trait. They can be any type of filter depending on how you set your coefficients.
		//This means they can be used as lowpass, bandpass, or highpass filters. You can even blend them together to create hybrid filters, which I do below.
		//cutoff = cutoffSet;
		double w = (2 * 3.14159) * (cutoff / sampleRate);
		double a = sin(w) / (2 * q);
		double cosw = cos(w);

		if (filterType >= 0) {//If filerType >= 0, I take the equation for a lowpass filter and a bandpass filter and "blend" them together using a weighted average. 
			biqCoefs[0] = (((filterType * (1 - cosw) / 2) + (1 - filterType) * a) / 2) / (1 + a);
			biqCoefs[1] = filterType * (1 - cosw / (1 + a));
			biqCoefs[2] = ((filterType * (1 - cosw) / 2) - (1 - filterType) * a) / (1 + a);
			biqCoefs[3] = (-2 * cosw) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}
		else { //If filterType < 0, I take the equation for a highpass filter and a bandpass filter and "blend" them together using a weighted average.
			double filtType = -filterType;
			biqCoefs[0] = (((filtType * (1 + cosw) / 2) + (1 - filtType) * a)) / (1 + a);
			biqCoefs[1] = (filtType * (-1 + cosw)) / (1 + a);
			biqCoefs[2] = (((filtType * (1 + cosw) / 2) - (1 - filtType) * (a))) / (1 + a);
			biqCoefs[3] = (-2 * cosw) / (1 + a);
			biqCoefs[4] = (1 - a) / (1 + a);
		}//This implementation allows the user too seamlessly blend between the three main filter types, giving extra control over the timbre of the synth.

		//gain compensation
		//Note: Biquad filters naturally will have their gain change depending on what frequency they are tuned to. 
		double gainLP = 1;
		double gainHP = 1;
		double gainBP = 1;
		double gain = 1;

		gainLP = (biqCoefs[0] + biqCoefs[1] + biqCoefs[2]) / (1 + biqCoefs[3] + biqCoefs[4]);//calculates gain at DC for the lowpass filter (DC = 0hz)

		gainHP = (biqCoefs[0] - biqCoefs[1] + biqCoefs[2]) / (1 - biqCoefs[3] + biqCoefs[4]);//calculates gain at Nyquist for the highpass filter (nyquist is the highest frequency in the signal)

		double real = biqCoefs[0] + biqCoefs[1] * cos(w) + biqCoefs[2] * cos(2 * w);//calculates the real portion of the gain at the center frequency for bandpass
		double imag = biqCoefs[1] * sin(w) + biqCoefs[2] * sin(2 * w);//Calculates the imaginary portion of the bandpass gain
		double mag = sqrt(real * real + imag * imag);//computes the magnitude by combining the real and imaginary portions together
		gainBP = mag / sqrt(1 + biqCoefs[3] * biqCoefs[3] + biqCoefs[4] * biqCoefs[4] + 2 * biqCoefs[3] * (1 + biqCoefs[4]) * cos(w) + 2 * biqCoefs[4] * cos(2 * w));//Calculates the gain at the cutoff frequency for the bandpass filter


		if (filterType >= 0) {//Calculates overall gain if the filter is bandpass, lowpass, or anything in-between
			gain = filterType * gainLP + (1 - filterType) * gainBP;
		}
		else {//Calculates for overall gain if the filter is highpass, bandpass, or anywhere in-between.
			gain = (-filterType)*gainHP + (1 + filterType) * gainBP;
		}

		if (gain != 0.0) { //dividing each B coefficient by the gain value to preserve unity-gain across the frequency spectrum
			biqCoefs[0] /= gain;
			biqCoefs[1] /= gain;
			biqCoefs[2] /= gain;
		}

	}

	void biquadFilter() { //This is a standard biquad filter. 
		if (filtOn) {
			for (int i = 0; i < bufferSize; i++) {
				for (int j = 0; j < channelCount; j++) {
					filterInReg[j][2] = filterInReg[j][1];
					filterInReg[j][1] = filterInReg[j][0];
					filterInReg[j][0] = localBuff[i * channelCount + j];//Setting the input registers

					localBuff[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Biquad filter equation

					filterOutReg[j][1] = filterOutReg[j][0];
					filterOutReg[j][0] = localBuff[i * channelCount + j]; //setting the output registers
				}
			}
		}
		
	}

	void setVars(GUIControls* guiControls);
};
