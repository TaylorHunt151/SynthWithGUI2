#pragma once

extern std::vector<char> notesHeld;
extern int oscAmpGoal;
extern char noteSet;
extern bool noteHeld[64];
extern int adsrState;
extern std::atomic<float> oscFreq;

float keyInputManager() {

	for (int keyNum = 27; keyNum < 91; keyNum++) {
		char keyLtr = static_cast<char>(keyNum);

		if (GetAsyncKeyState(keyNum) & 0x8000) {
			if (!noteHeld[keyNum - 27]) {
				noteSet = keyLtr;
				noteHeld[keyNum - 27] = true;
				notesHeld.push_back(keyLtr); //If you press a note that isn't already pressed, it gets added to the vector
				oscAmpGoal = 1; //Triggers the ADSR to start when set to 1
				adsrState = 0; //resets the envelope to the attack phase
			}
		}
		else {
			if (noteHeld[keyNum - 27]) {
				noteHeld[keyNum - 27] = false;
				notesHeld.erase(std::remove(notesHeld.begin(), notesHeld.end(), keyLtr), notesHeld.end());  //If u let go of a note, it gets deleted from the vector.
				if (keyLtr == noteSet) {
					adsrState = 3;
					oscAmpGoal = 0;
				}
			}
		}
	}

	return 0.0;

}


void noteSetter() { //This sets the frequency of the oscillator. If no note keys are pressed, the amplitude goes to zero

	switch (noteSet) {
		case 'Z':
			oscFreq = 2 * 65.41;


			break;
		case 'S':
			oscFreq = 2 * 69.3;


			break;
		case 'X':
			oscFreq = 2 * 73.42;


			break;
		case 'D':
			oscFreq = 2 * 77.78;


			break;
		case 'C':
			oscFreq = 2 * 82.41;


			break;
		case 'V':
			oscFreq = 2 * 87.31;


			break;
		case 'G':
			oscFreq = 2 * 92.5;


			break;
		case 'B':
			oscFreq = 2 * 98;


			break;
		case 'H':
			oscFreq = 2 * 103.83;


			break;
		case 'N':
			oscFreq = 2 * 110;


			break;
		case 'J':
			oscFreq = 2 * 116.54;


			break;
		case 'M':
			oscFreq = 2 * 123.47;


			break;
		case 'Q':
			oscFreq = 2 * 130.81;


			break;
		case '2':
			oscFreq = 2 * 138.59;


			break;
		case 'W':
			oscFreq = 2 * 146.83;


			break;
		case '3':
			oscFreq = 2 * 155.56;


			break;
		case 'E':
			oscFreq = 2 * 164.81;


			break;
		case 'R':
			oscFreq = 2 * 174.61;


			break;
		case '5':
			oscFreq = 2 * 185;


			break;
		case 'T':
			oscFreq = 2 * 196;


			break;
		case '6':
			oscFreq = 2 * 207.65;


			break;
		case 'Y':
			oscFreq = 2 * 220;


			break;
		case '7':
			oscFreq = 2 * 233.08;


			break;
		case 'U':
			oscFreq = 2 * 246.94;


			break;
		case 'I':
			oscFreq = 2 * 261.63;


			break;
		case '9':
			oscFreq = 2 * 277.78;


			break;
		case 'O':
			oscFreq = 2 * 293.66;


			break;
		case '0':
			oscFreq = 2 * 311.13;


			break;
		case 'P':
			oscFreq = 2 * 329.63;


			break;

		default:
			oscAmpGoal = 0;
			adsrState = 3;
			break;

		}
}