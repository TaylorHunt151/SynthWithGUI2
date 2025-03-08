#pragma once

#include <wx/wx.h>
#include <vector>
#include <algorithm>
#include <mutex>
#include "VoiceFunctions.h"

extern std::vector<char> notesHeld;
extern int oscAmpGoal;
extern std::vector<char> noteSet;
extern bool noteHeld[64];
extern double oscFreq;
extern voice* voices;
int voiceIndex = 0;
std::mutex keyInputMutex;

class KeyInputManager : public wxEvtHandler {
public:
	void OnKeyDown(wxKeyEvent& event) {
		std::lock_guard<std::mutex> lock(keyInputMutex);
		int keyCode = event.GetKeyCode();
		if (keyCode >= 'A' && keyCode <= 'Z') {
			char keyLtr = static_cast<char>(keyCode);
			if (!noteHeld[keyCode - 'A']) {
				noteSet[notesHeld.size()] = keyLtr;
				noteHeld[keyCode - 'A'] = true;
				notesHeld.push_back(keyLtr);
				voiceIndex = notesHeld.size() - 1;
				voices[voiceIndex].oscAmpGoal = 1;
				voices[voiceIndex].adsrState = 0;
			}
		}
		event.Skip();
	}

	void OnKeyUp(wxKeyEvent& event) {
		std::lock_guard<std::mutex> lock(keyInputMutex);
		int keyCode = event.GetKeyCode();
		if (keyCode >= 'A' && keyCode <= 'Z') {
			char keyLtr = static_cast<char>(keyCode);
			if (noteHeld[keyCode - 'A']) {
				noteHeld[keyCode - 'A'] = false;
				notesHeld.erase(std::remove(notesHeld.begin(), notesHeld.end(), keyLtr), notesHeld.end());
				voiceIndex = notesHeld.size();
				voices[voiceIndex].oscAmpGoal = 0;
				voices[voiceIndex].adsrState = 3;
			}
		}
		event.Skip();
	}
};



void noteSetter(int oscNum) { //This sets the frequency of the oscillator. If no note keys are pressed, the amplitude goes to zero

	switch (noteSet[oscNum]) {
		case 'Z':
			voices[oscNum].oscFreq = 3 * 65.41;

			break;
		case 'S':
			voices[oscNum].oscFreq = 3 * 69.3;

			break;
		case 'X':
			voices[oscNum].oscFreq = 3 * 73.42;

			break;
		case 'D':
			voices[oscNum].oscFreq = 3 * 77.78;

			break;
		case 'C':
			voices[oscNum].oscFreq = 3 * 82.41;

			break;
		case 'V':
			voices[oscNum].oscFreq = 3 * 87.31;

			break;
		case 'G':
			voices[oscNum].oscFreq = 3 * 92.5;

			break;
		case 'B':
			voices[oscNum].oscFreq = 3 * 98;

			break;
		case 'H':
			voices[oscNum].oscFreq = 3 * 103.83;

			break;
		case 'N':
			voices[oscNum].oscFreq = 3 * 110;

			break;
		case 'J':
			voices[oscNum].oscFreq = 3 * 116.54;

			break;
		case 'M':
			voices[oscNum].oscFreq = 3 * 123.47;

			break;
		case 'Q':
			voices[oscNum].oscFreq = 3 * 130.81;

			break;
		case '2':
			voices[oscNum].oscFreq = 3 * 138.59;

			break;
		case 'W':
			voices[oscNum].oscFreq = 3 * 146.83;

			break;
		case '3':
			voices[oscNum].oscFreq = 3 * 155.56;

			break;
		case 'E':
			voices[oscNum].oscFreq = 3 * 164.81;

			break;
		case 'R':
			voices[oscNum].oscFreq = 3 * 174.61;

			break;
		case '5':
			voices[oscNum].oscFreq = 3 * 185;

			break;
		case 'T':
			voices[oscNum].oscFreq = 3 * 196;

			break;
		case '6':
			voices[oscNum].oscFreq = 3 * 207.65;

			break;
		case 'Y':
			voices[oscNum].oscFreq = 3 * 220;

			break;
		case '7':
			voices[oscNum].oscFreq = 3 * 233.08;

			break;
		case 'U':
			voices[oscNum].oscFreq = 3 * 246.94;

			break;
		case 'I':
			voices[oscNum].oscFreq = 3 * 261.63;

			break;
		case '9':
			voices[oscNum].oscFreq = 3 * 277.78;

			break;
		case 'O':
			voices[oscNum].oscFreq = 3 * 293.66;

			break;
		case '0':
			voices[oscNum].oscFreq = 3 * 311.13;

			break;
		case 'P':
			voices[oscNum].oscFreq = 3 * 329.63;

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