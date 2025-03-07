#pragma once
#include <random>
extern float oscAmpMultiplier;
extern int oscAmpGoal;
extern int adsrState;

extern std::vector<std::vector<float>> filterOutReg;
extern std::vector<std::vector<float>> filterInReg;

//Initialize random generator
std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(-1, 1);


float oscillator(double *buffer, int buffSize, int channels, float freq, int type, float amp, float phase) {

	switch (type) {
		case 0: //Sine wave
			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++)
				{
					buffer[i * channels + j] = amp * sin(2 * M_PI * freq * phase); //Calculates sine wave values
					phase += 1.0 / 44100.0; //Keeps track of the phase
					if (phase >= 1.0) {
						phase = 0;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 1: //Square wave
			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++)
				{
					buffer[i * channels + j] = amp * (phase < 0.5 ? 1 : -1); //Calculates square wave values
					

					phase += freq / 44100.0; //Keeps track of the phase
					if (phase >= 1.0) {
						phase = 0;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 2: //Sawtooth wave
			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++)
				{
					buffer[i * channels + j] = amp * (2 * phase - 1); //Calculates sawtooth wave values
					phase += freq / 44100.0; //Keeps track of the phase
					if (phase >= 1.0) {
						phase = 0;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 3: //Triangle wave
			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++)
				{
					buffer[i * channels + j] = amp * (2 * abs(2 * phase - 1) - 1); //Calculates triangle wave values
					phase += freq / 44100.0; //Keeps track of the phase
					if (phase >= 1.0) {
						phase = 0;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 4: //noise

			for (int i = 0; i < buffSize; i++) {
				for (int j = 0; j < channels; j++) {
					buffer[i * channels + j] = amp * (distribution(generator) * 2 - 1); //Generates random numbers between -1 and 1 to create noise.
					//Noise is useful for making percussive sounds and ambience.
				}
			}

	}
	return phase;
}

void envelope(double *buffer, int buffSize, int channels, int sampleRate, float attack, float decay, float sustain, float release, int adsrState) {
	attack *= sampleRate;
	decay *= sampleRate;
	release *= sampleRate;
	for (int i = 0; i < buffSize; i++) {
		for (int j = 0; j < channels; j++) {
			if (oscAmpGoal == 1) {
				if (adsrState == 0) { // attack stage
					if (oscAmpMultiplier < 1) {
						oscAmpMultiplier += 1 / attack;


					}
					else {
						adsrState = 1;
						
					}
				}
				if (adsrState == 1) { // decay stage. NOTE: this doesn't work for some reason. If you can figure out why, I will give you a cookie.
					if (oscAmpMultiplier > sustain) {
						oscAmpMultiplier -= 1 / decay;

					}
					else {
						wxLogStatus("AHHHHHHHHHHHHHHH");
						oscAmpMultiplier = sustain;
						adsrState = 2;
					}
				}
				if (adsrState == 3) { // release stage
					if (oscAmpMultiplier > 0) {
						oscAmpMultiplier -= 1 / release;
						if (oscAmpMultiplier < 0) {
							oscAmpMultiplier = 0;
						}
					}
				}
			}
			else {
				if (adsrState == 3) { // release stage
					if (oscAmpMultiplier > 0) {
						oscAmpMultiplier -= 1 / release;
						if (oscAmpMultiplier < 0) {
							oscAmpMultiplier = 0;
						}
					}
				}
				else {
					oscAmpMultiplier = 0;
				}
			}

			buffer[i * channels + j] *= oscAmpMultiplier;

			}
		}
	}
	
void biquadCoefs(int sampleRate, float cutoff, float q, float filterType, float *coefs) {

	float w = (2 * 3.14159) * (cutoff / sampleRate);
	float a = sin(w) / (2 * q);

	if (filterType <= 0) {
		coefs[0] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (a)) / 2) / (1 + a);
		coefs[1] = (1 - cos(w)) / (1 + a);
		coefs[2] = ((((-1) * filterType * (1 - cos(w)) / 2) + (1 - filterType * (-1)) * (0 - a)) / 2) / (1 + a);
		//coefs[3] = 1 + a;
		coefs[3] = (-2 * cos(w)) / (1 + a);
		coefs[4] = (1 - a) / (1 + a);
	}
	else {
		coefs[0] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (a)) / 2) / (1 + a);
		coefs[1] = (filterType * (-1 + cos(w))) / (1 + a);
		coefs[2] = (((filterType * (1 + cos(w)) / 2) + (1 - filterType) * (-a)) / 2) / (1 + a);
		//coefs[3] = 1 + a;
		coefs[3] = (-2 * cos(w)) / (1 + a);
		coefs[4] = (1 - a) / (1 + a);
	}

}

void filter(double *buffer, int buffSize, int channelCount, int sampleRate, float *biqCoefs) {

	for (int i = 0; i < buffSize; i++) {
		for (int j = 0; j < channelCount; j++) {
			filterInReg[j][2] = filterInReg[j][1];
			filterInReg[j][1] = filterInReg[j][0];
			filterInReg[j][0] = buffer[i * channelCount + j];

			buffer[i * channelCount + j] = filterInReg[j][0] * biqCoefs[0] + filterInReg[j][1] * biqCoefs[1] + filterInReg[j][2] * biqCoefs[2] - filterOutReg[j][0] * biqCoefs[3] - filterOutReg[j][1] * biqCoefs[4]; //Filter math equation

			filterOutReg[j][1] = filterOutReg[j][0];
			filterOutReg[j][0] = buffer[i * channelCount + j];
		}
	}

}