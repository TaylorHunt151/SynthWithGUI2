#pragma once

extern float oscAmpMultiplier;
extern int oscAmpGoal;
extern int adsrState;

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

						if (oscAmpMultiplier > 1) {
							oscAmpMultiplier = 1;
						}

					}
					else {
						adsrState = 1;
					}
				}
				if (adsrState == 1) { // decay stage
					if (oscAmpMultiplier > sustain) {
						//wxLogStatus("FUCK");
						oscAmpMultiplier -= 1 / decay;

					}
					else {
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
	//return adsrState;
	
