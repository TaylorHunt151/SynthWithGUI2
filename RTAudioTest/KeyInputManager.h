#pragma once

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

        if ((currentKey >= 'A' && currentKey <= 'Z') || (currentKey >= '0' && currentKey <= '9')) { //Checks if key is a valid input
            if (keyToVoiceMap.find(currentKey) == keyToVoiceMap.end()) { //If the key is not already being held
                noteSet[voiceIncrement] = currentKey; //Sets the voice's note to the current key
                keyToVoiceMap[currentKey] = voiceIncrement; //Map the key to the current voice
                voices[voiceIncrement].adsrState = 0;
                voices[voiceIncrement].oscAmpGoal = 1;
                noteSetter(voiceIncrement);

                if (voiceIncrement < (voiceCount - 1)) { //Increments the voice tracker, preparing the input manager to set the next note.
                    voiceIncrement++;
                }
                else { //Round-robins the voice tracker
                    voiceIncrement = 0;
                }
            }
        }

        event.Skip(); //Skip the event to allow further processing
    }

	//Handles key up events
    void OnKeyUp(wxKeyEvent& event) {
        currentKey = event.GetUnicodeKey(); //Set current key

        if ((currentKey >= 'A' && currentKey <= 'Z') || (currentKey >= '0' && currentKey <= '9')) { //Checks if key is a valid input
            auto it = keyToVoiceMap.find(currentKey);
            if (it != keyToVoiceMap.end()) { //If the key is found in the map
                int voiceIndex = it->second;
                noteSet[voiceIndex] = ' ';
                voices[voiceIndex].oscAmpGoal = 0;
                voices[voiceIndex].adsrState = 3;
                keyToVoiceMap.erase(it); //Remove the key from the map
                noteSetter(voiceIndex);
            }
        }

        event.Skip(); //Skip the event to allow further processing
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
        }
    }
	
};
