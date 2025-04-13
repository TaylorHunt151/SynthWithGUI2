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
#include <Windows.h>


extern int voiceCount;

extern int oscAmpGoal;
extern std::vector<char> noteSet;

extern double oscFreq;
extern voice* voices;
int voiceIndex = 0;

extern int voiceCount;



class KeyInputManager : public wxEvtHandler {
public:

	bool noteHeld[64]; //An array that says whether each note is currently being held down
	std::vector<char> noteSet; //A vector containing all notes currently being held in order of when they were pressed. When a note is released, it should be replaced with ' '. 
    int noteIndex; //Keeps track of which voice is to be assigned or unassigned
	int voiceIncrement = 0; //keeps track of which voice is being edited.

	char currentKey; //stores the key that was just pressed.

	KeyInputManager(){
		//Initialize noteSet with one element per voice
		noteSet.resize(voiceCount);

		for (int i = 0; i < voiceCount; ++i) {
			noteSet[i] = ' '; //Fill noteset with default value
		}
	} //Initialize noteSet with voiceCount

    void keyScan() {//Scans all keys for inputs. This is necessary because the default wxEVT_KEY_DOWN and wxEVT_KEY_UP events were causing weird glitches, so I implemented my own key press detection.

        for (int keyCode = '0'; keyCode <= 'Z'; keyCode++) {
            int keyMin = '0';
            bool keyHeld = GetAsyncKeyState(keyCode);
            bool keyAlreadyDown = noteHeld[keyCode - keyMin];
            noteHeld[keyCode - keyMin] = keyHeld;

            if (keyHeld && !keyAlreadyDown) {
                //wxLogMessage("AHHHH");
                OnKeyDown(keyCode);
            }
            else if (!keyHeld && keyAlreadyDown) {
                OnKeyUp(keyCode);
            }

        }
    }

    void OnKeyDown(int currentKey) {
        char keyChar = toupper(currentKey);
        bool keyPressed = false;
        if (keyChar >= '0' && keyChar <= 'Z') {
            int index = keyChar - '0';
            // Only set if not already pressed.
            for (int i = 0; i < voiceCount; i++) {//Check if the key is pressed
                if (noteSet[i] == keyChar) {
                    keyPressed = true;
                }
            }
            if (!keyPressed) {//If the note is not already being held down, assign it to a voice and start the envelope
                noteSet[noteIndex] = keyChar;
                voices[noteIndex].adsrState = 0;
                voices[noteIndex].oscAmpGoal = 1;
                noteSetter(noteIndex);
                noteIndex++;
                if (noteIndex >= voiceCount) {
                    noteIndex = 0;
                }
            }
        }

    }

    void OnKeyUp(int currentKey) {//for when the user releases a note
        char keyChar = toupper(currentKey);
        if (keyChar >= '0' && keyChar <= 'Z') {
            for (int i = 0; i < voiceCount; i++) {
                if (noteSet[i] == keyChar) {
                    noteSet[i] = ' ';
                    voices[i].oscAmpGoal = 0;
                    voices[i].adsrState = 3;
                    noteSetter(i);
                }
            }
        }

    }


private:
    void noteSetter(int oscNum) { //This sets the frequency of the oscillator. If no note keys are pressed, the amplitude goes to zero


        switch (noteSet[oscNum]) { //Use the instance to access noteSet
        case 'Z': voices[oscNum].oscFreq = 65.41;  voices[oscNum].semitones = -21; break;
        case 'S': voices[oscNum].oscFreq = 69.3; voices[oscNum].semitones = -20; break;
        case 'X': voices[oscNum].oscFreq = 73.42; voices[oscNum].semitones = -19; break;
        case 'D': voices[oscNum].oscFreq = 77.78; voices[oscNum].semitones = -18; break;
        case 'C': voices[oscNum].oscFreq = 82.41; voices[oscNum].semitones = -17; break;
        case 'V': voices[oscNum].oscFreq = 87.31; voices[oscNum].semitones = -16; break;
        case 'G': voices[oscNum].oscFreq = 92.5; voices[oscNum].semitones = -15; break;
        case 'B': voices[oscNum].oscFreq = 98; voices[oscNum].semitones = -14; break;
        case 'H': voices[oscNum].oscFreq = 103.83; voices[oscNum].semitones = -13; break;
        case 'N': voices[oscNum].oscFreq = 110; voices[oscNum].semitones = -12; break;
        case 'J': voices[oscNum].oscFreq = 116.54; voices[oscNum].semitones = -11; break;
        case 'M': voices[oscNum].oscFreq = 123.47; voices[oscNum].semitones = -10; break;
        case 'Q': voices[oscNum].oscFreq = 130.81; voices[oscNum].semitones = -9; break;
        case '2': voices[oscNum].oscFreq = 138.59; voices[oscNum].semitones = -8; break;
        case 'W': voices[oscNum].oscFreq = 146.83; voices[oscNum].semitones = -7; break;
        case '3': voices[oscNum].oscFreq = 155.56; voices[oscNum].semitones = -6; break;
        case 'E': voices[oscNum].oscFreq = 164.81; voices[oscNum].semitones = -5; break;
        case 'R': voices[oscNum].oscFreq = 174.61; voices[oscNum].semitones = -4; break;
        case '5': voices[oscNum].oscFreq = 185; voices[oscNum].semitones = -3; break;
        case 'T': voices[oscNum].oscFreq = 196; voices[oscNum].semitones = -2; break;
        case '6': voices[oscNum].oscFreq = 207.65; voices[oscNum].semitones = -1;  break;
        case 'Y': voices[oscNum].oscFreq = 220; voices[oscNum].semitones = 0; break;
        case '7': voices[oscNum].oscFreq = 233.08; voices[oscNum].semitones = 1; break;
        case 'U': voices[oscNum].oscFreq = 246.94; voices[oscNum].semitones = 2; break;
        case 'I': voices[oscNum].oscFreq = 261.63; voices[oscNum].semitones = 3; break;
        case '9': voices[oscNum].oscFreq = 277.78; voices[oscNum].semitones = 4; break;
        case 'O': voices[oscNum].oscFreq = 293.66; voices[oscNum].semitones = 5; break;
        case '0': voices[oscNum].oscFreq = 311.13; voices[oscNum].semitones = 6; break;
        case 'P': voices[oscNum].oscFreq = 329.63; voices[oscNum].semitones = 7; break;

        default:
            voices[oscNum].oscAmpGoal = 0;
            voices[oscNum].adsrState = 3;
            break;

        }

        if (voices[oscNum].keyTrack) {
            voices[oscNum].cutoff = voices[oscNum].cutoffSet * pow(2, voices[oscNum].semitones / 12);
        }
        else {
            voices[oscNum].cutoff = voices[oscNum].cutoffSet;
        }
        
    }
	
};
