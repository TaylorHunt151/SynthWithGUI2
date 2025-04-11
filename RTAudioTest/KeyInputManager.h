#pragma once

/****************************************************************
* NOTE: This class does NOT get any GUI elements. It is pure
* back-end stuff. Don't worry about this file yet. We may do
* something with it later, but only if we have time.
* 
* -Tay
*****************************************************************/

#include <wx/wx.h>
#include <vector>
#include <algorithm>
#include "VoiceFunctions.h"
#include <unordered_map>


extern int voiceCount;

extern std::vector<char> notesHeld;
extern int oscAmpGoal;
extern std::vector<char> noteSet;
extern double oscFreq;
extern voice* voices;
int voiceIndex = 0;

extern int voiceCount;



class KeyInputManager : public wxEvtHandler {
public:

	//bool noteHeld[64]; //An array that says whether each note is currently being held down
	std::vector<char> noteSet; //A vector containing all notes currently being held in order of when they were pressed. When a note is released, it should be replaced with ' '. 
	int voiceIncrement = 0; //keeps track of which voice is being edited.
    std::unordered_map<char, int> keyToVoiceMap; //Map to track which key is assigned to which voice

	char currentKey; //stores the key that was just pressed.

	KeyInputManager(){
		//Initialize noteSet with one element per voice
		noteSet.resize(voiceCount);

		for (int i = 0; i < voiceCount; ++i) {
			noteSet[i] = ' '; //Fill noteset with default value
		}
	} //Initialize noteSet with voiceCount

    void OnKeyDown(wxKeyEvent& event) {
        currentKey = event.GetUnicodeKey();
        char keyChar = toupper(currentKey);
        if (keyChar >= 'A' && keyChar <= 'P') {
            int index = keyChar - 'A';
            // Only set if not already pressed.
            if (noteSet[index] == ' ') {
                noteSet[index] = keyChar;

                voices[index].adsrState = 0;
                voices[index].oscAmpGoal = 1;
                noteSetter(index);
            }
        }
        event.Skip();
    }

    void OnKeyUp(wxKeyEvent& event) {
        currentKey = event.GetUnicodeKey();
        char keyChar = toupper(currentKey);
        if (keyChar >= 'A' && keyChar <= 'P') {
            int index = keyChar - 'A';
            noteSet[index] = ' ';
            voices[index].oscAmpGoal = 0;
            voices[index].adsrState = 3;
            noteSetter(index);
        }
        event.Skip();

    }


private:
    void noteSetter(int oscNum) { //This sets the frequency of the oscillator. If no note keys are pressed, the amplitude goes to zero


        switch (noteSet[oscNum]) { //Use the instance to access noteSet
        case 'Z': voices[oscNum].oscFreq = 65.41; break;
        case 'S': voices[oscNum].oscFreq = 69.3; break;
        case 'X': voices[oscNum].oscFreq = 73.42; break;
        case 'D': voices[oscNum].oscFreq = 77.78; break;
        case 'C': voices[oscNum].oscFreq = 82.41; break;
        case 'V': voices[oscNum].oscFreq = 87.31; break;
        case 'G': voices[oscNum].oscFreq = 92.5; break;
        case 'B': voices[oscNum].oscFreq = 98; break;
        case 'H': voices[oscNum].oscFreq = 103.83; break;
        case 'N': voices[oscNum].oscFreq = 110; break;
        case 'J': voices[oscNum].oscFreq = 116.54; break;
        case 'M': voices[oscNum].oscFreq = 123.47; break;
        case 'Q': voices[oscNum].oscFreq = 130.81; break;
        case '2': voices[oscNum].oscFreq = 138.59; break;
        case 'W': voices[oscNum].oscFreq = 146.83; break;
        case '3': voices[oscNum].oscFreq = 155.56; break;
        case 'E': voices[oscNum].oscFreq = 164.81; break;
        case 'R': voices[oscNum].oscFreq = 174.61; break;
        case '5': voices[oscNum].oscFreq = 185; break;
        case 'T': voices[oscNum].oscFreq = 196; break;
        case '6': voices[oscNum].oscFreq = 207.65; break;
        case 'Y': voices[oscNum].oscFreq = 220; break;
        case '7': voices[oscNum].oscFreq = 233.08; break;
        case 'U': voices[oscNum].oscFreq = 246.94; break;
        case 'I': voices[oscNum].oscFreq = 261.63; break;
        case '9': voices[oscNum].oscFreq = 277.78; break;
        case 'O': voices[oscNum].oscFreq = 293.66; break;
        case '0': voices[oscNum].oscFreq = 311.13; break;
        case 'P': voices[oscNum].oscFreq = 329.63; break;

        default:
            voices[oscNum].oscAmpGoal = 0;
            voices[oscNum].adsrState = 3;
            break;

        }

        for (int i = 0; i < voiceCount; i++) {
            if (voices[i].keyTrack) {
                voices[i].cutoff = (voices[i].oscFreq - 220) + voices[i].cutoffSet;
            }
            else {
                voices[i].cutoff = voices[i].cutoffSet;
            }
        }
    }
	
};
