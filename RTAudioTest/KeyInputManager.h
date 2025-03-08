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
extern int voiceCount;

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
			voices[oscNum].oscFreq = 65.41;

			break;
		case 'S':
			voices[oscNum].oscFreq = 69.3;

			break;
		case 'X':
			voices[oscNum].oscFreq = 73.42;

			break;
		case 'D':
			voices[oscNum].oscFreq = 77.78;

			break;
		case 'C':
			voices[oscNum].oscFreq = 82.41;

			break;
		case 'V':
			voices[oscNum].oscFreq = 87.31;

			break;
		case 'G':
			voices[oscNum].oscFreq = 92.5;

			break;
		case 'B':
			voices[oscNum].oscFreq = 98;

			break;
		case 'H':
			voices[oscNum].oscFreq = 103.83;

			break;
		case 'N':
			voices[oscNum].oscFreq = 110;

			break;
		case 'J':
			voices[oscNum].oscFreq = 116.54;

			break;
		case 'M':
			voices[oscNum].oscFreq = 123.47;

			break;
		case 'Q':
			voices[oscNum].oscFreq = 130.81;

			break;
		case '2':
			voices[oscNum].oscFreq = 138.59;

			break;
		case 'W':
			voices[oscNum].oscFreq = 146.83;

			break;
		case '3':
			voices[oscNum].oscFreq = 155.56;

			break;
		case 'E':
			voices[oscNum].oscFreq = 164.81;

			break;
		case 'R':
			voices[oscNum].oscFreq = 174.61;

			break;
		case '5':
			voices[oscNum].oscFreq = 185;

			break;
		case 'T':
			voices[oscNum].oscFreq = 196;

			break;
		case '6':
			voices[oscNum].oscFreq = 207.65;

			break;
		case 'Y':
			voices[oscNum].oscFreq = 220;

			break;
		case '7':
			voices[oscNum].oscFreq = 233.08;

			break;
		case 'U':
			voices[oscNum].oscFreq = 246.94;

			break;
		case 'I':
			voices[oscNum].oscFreq = 261.63;

			break;
		case '9':
			voices[oscNum].oscFreq = 277.78;

			break;
		case 'O':
			voices[oscNum].oscFreq = 293.66;

			break;
		case '0':
			voices[oscNum].oscFreq = 311.13;

			break;
		case 'P':
			voices[oscNum].oscFreq = 329.63;

			break;

		default:
			voices[notesHeld.size()].oscAmpGoal = 0;
			voices[notesHeld.size()].adsrState = 3;
			break;

		}

		for (int i = 0; i < voiceCount; i++) {
			if (voices[i].keyTrack) {
				voices[i].cutoff = (voices[i].oscFreq - 220) + voices[i].cutoffSet;
			}
		}
}