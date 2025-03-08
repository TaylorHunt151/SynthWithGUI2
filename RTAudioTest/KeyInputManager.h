#pragma once

extern std::vector<char> notesHeld;
extern int oscAmpGoal;
extern std::vector<char> noteSet;
extern bool noteHeld[64];
//extern int adsrState;
extern float oscFreq;
//extern std::atomic<bool> keyTrack;
//extern std::atomic<float> cutoff;
extern voice* voices;
int voiceIndex = 0;

void keyInputManager() {
	for (int keyNum = 27; keyNum < 91; keyNum++) {
		char keyLtr = static_cast<char>(keyNum);

		if (GetAsyncKeyState(keyNum) & 0x8000) {
			if (!noteHeld[keyNum - 27]) {
				noteSet[notesHeld.size()] = keyLtr;
				noteHeld[keyNum - 27] = true;
				notesHeld.push_back(keyLtr); // If you press a note that isn't already pressed, it gets added to the vector
				voiceIndex = notesHeld.size() - 1; //Tracks which voice is currently being set by the input manager
				voices[voiceIndex].oscAmpGoal = 1; // Triggers the ADSR to start when set to 1
				voices[voiceIndex].adsrState = 0; // Resets the envelope to the attack phase
			}
		}
		else {
			if (noteHeld[keyNum - 27]) {
				noteHeld[keyNum - 27] = false;
				notesHeld.erase(std::remove(notesHeld.begin(), notesHeld.end(), keyLtr), notesHeld.end()); // If you let go of a note, it gets deleted from the vector.
				voiceIndex = notesHeld.size(); 
				voices[voiceIndex].oscAmpGoal = 0; // Triggers the ADSR to release when set to 0
				voices[voiceIndex].adsrState = 3; // Sets the envelope to the release phase
			}
		}
	}
}




void noteSetter(int oscNum) { //This sets the frequency of the oscillator. If no note keys are pressed, the amplitude goes to zero

	switch (noteSet[oscNum]) {
		case 'Z':
			voices[oscNum].oscFreq = 2 * 65.41;

			break;
		case 'S':
			voices[oscNum].oscFreq = 2 * 69.3;

			break;
		case 'X':
			voices[oscNum].oscFreq = 2 * 73.42;

			break;
		case 'D':
			voices[oscNum].oscFreq = 2 * 77.78;

			break;
		case 'C':
			voices[oscNum].oscFreq = 2 * 82.41;

			break;
		case 'V':
			voices[oscNum].oscFreq = 2 * 87.31;

			break;
		case 'G':
			voices[oscNum].oscFreq = 2 * 92.5;

			break;
		case 'B':
			voices[oscNum].oscFreq = 2 * 98;

			break;
		case 'H':
			voices[oscNum].oscFreq = 2 * 103.83;

			break;
		case 'N':
			voices[oscNum].oscFreq = 2 * 110;

			break;
		case 'J':
			voices[oscNum].oscFreq = 2 * 116.54;

			break;
		case 'M':
			voices[oscNum].oscFreq = 2 * 123.47;

			break;
		case 'Q':
			voices[oscNum].oscFreq = 2 * 130.81;

			break;
		case '2':
			voices[oscNum].oscFreq = 2 * 138.59;

			break;
		case 'W':
			voices[oscNum].oscFreq = 2 * 146.83;

			break;
		case '3':
			voices[oscNum].oscFreq = 2 * 155.56;

			break;
		case 'E':
			voices[oscNum].oscFreq = 2 * 164.81;

			break;
		case 'R':
			voices[oscNum].oscFreq = 2 * 174.61;

			break;
		case '5':
			voices[oscNum].oscFreq = 2 * 185;

			break;
		case 'T':
			voices[oscNum].oscFreq = 2 * 196;

			break;
		case '6':
			voices[oscNum].oscFreq = 2 * 207.65;

			break;
		case 'Y':
			voices[oscNum].oscFreq = 2 * 220;

			break;
		case '7':
			voices[oscNum].oscFreq = 2 * 233.08;

			break;
		case 'U':
			voices[oscNum].oscFreq = 2 * 246.94;

			break;
		case 'I':
			voices[oscNum].oscFreq = 2 * 261.63;

			break;
		case '9':
			voices[oscNum].oscFreq = 2 * 277.78;

			break;
		case 'O':
			voices[oscNum].oscFreq = 2 * 293.66;

			break;
		case '0':
			voices[oscNum].oscFreq = 2 * 311.13;

			break;
		case 'P':
			voices[oscNum].oscFreq = 2 * 329.63;

			break;

		default:
			voices[notesHeld.size()].oscAmpGoal = 0;
			voices[notesHeld.size()].adsrState = 3;
			break;

		}
		//if (keyTrack) {
		//	cutoff.store(oscFreq);
		//}
}