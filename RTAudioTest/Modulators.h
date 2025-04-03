#pragma once

/*******************************************************************
* NOTES FOR GUI: There are two LFO objects created in main.cpp. They
* each will require their own section in the GUI. You may access
* their attributes like so: "LFOs[i].attribute", where i is the index
* of the LFO (ranging from 0 to 1) and "attribute" is the name of
* the attribute you wanna access.
* 
* At the beginning of the LFO class, you will find several atomic
* attributes. These should each have their own GUI element. Next to
* the attributes, you will find a comment outlining what type of GUI
* element they should have, what the range of values should be, etc.
* If anything is unclear, don't hesitate to reach out to me!
* 
* -Tay
********************************************************************/
#include <cmath>


//Initialize random generator


extern int channelCount;
extern std::atomic<int> bufferSize; //Don't make a UI element for this

class LFO {
public: 

	//LFO PARAMETERS
	std::atomic<double> freq = .43; //Changeable, range from 0.1 to 20
	std::atomic<double> amp = .05; //Changeable, range from -1 to 1
	std::atomic<int> type = 0; //Changeable via dropdown menu, range 0 to 4
	std::atomic<int> carrier = 0; //Changeable via dropdown menu. Decides which parameter the LFO will modulate. 0 is for nothing, 1 is for oscillator freq.
	std::atomic<double> phaseOffset = 0; //Changeable via dropdown menu. Range from -1 to 1
	double phase = 0;
	std::vector<double> localBuff;

	LFO() {
		localBuff.resize((bufferSize.load()), 0);
	}

	void lfoGen() {
		switch (type) {
		case 0: //Sine wave
			for (int i = 0; i < bufferSize; i++) {

				localBuff[i] = amp * sin(2 * 3.14159 * freq * phase); //Calculates sine wave values
				phase += 1 / 44100.0; //Keeps track of the phase
				if (phase >= 1) {
					phase = 0;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.

				}
			}
			break;
		case 1: //Square wave
			for (int i = 0; i < bufferSize; i++) {

				localBuff[i] = amp * (phase < 0.5 ? 1 : -1); //Calculates square wave values

				phase += freq / 44100.0; //Keeps track of the phase
				if (phase >= 1.0) {
					phase = 0;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.

				}
			}
			break;
		case 2: //Sawtooth wave
			for (int i = 0; i < bufferSize; i++) {
				for (int j = 0; j < channelCount; j++)
				{
					localBuff[i * channelCount + j] = amp * (2 * phase - 1); //Calculates sawtooth wave values
					phase += freq / 44100.0; //Keeps track of the phase

					if (phase >= 1.0) {
						phase = 0;//This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;
		case 3: //Triangle wave
			for (int i = 0; i < bufferSize; i++) {
				for (int j = 0; j < channelCount; j++) {
					localBuff[i * channelCount + j] = amp * (2 * abs(2 * phase - 1) - 1); //Calculates triangle wave values
					phase += freq / 44100.0; //Keeps track of the phase
					if (phase >= 1.0) {
						phase = 0; //This resets phase to 0 when it reaches 1. This is necessary to prevent phase from becoming too large and causing an overflow error.
					}
				}
			}
			break;

			
		}
	}
};