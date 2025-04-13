#pragma once

#include "VoiceFunctions.h"
#include "GUIStruct.h"

void voice::setVars(GUIControls* guiControls) { //In order to set the variables from outside the audio thread, I use global variables as a sort-of middleman. This is super hacky. I'm sure there's a faster and better way to do this, but I'm too tired to care.

	//OSC1
	oscType = guiControls->oscTypeMenu->GetSelection();
	oscAmp = guiControls->oscVolKnob->GetValue();
	oscPhaseOffset = guiControls->oscPhsKnob->GetValue();
	oscPitchShift = guiControls->oscPitchKnob->GetValue();

	//OSC2
	osc2Type = guiControls->osc2TypeMenu->GetSelection();
	osc2Amp = guiControls->osc2VolKnob->GetValue();
	osc2PhaseOffset = guiControls->osc2PhsKnob->GetValue();
	osc2PitchShift = guiControls->osc2PitchKnob->GetValue();

	//ENVELOPE
	attack = guiControls->envAtk->GetValue();
	decay = guiControls->envDec->GetValue();
	sustain = guiControls->envSus->GetValue();
	release = guiControls->envRel->GetValue();

	//FILTER
	filtOn = guiControls->filtOnChoice->GetSelection();
	keyTrack = guiControls->filtKeyTrack->GetSelection();
	cutoffSet = guiControls->cutoffKnob->GetValue();
	q = guiControls->filtQKnob->GetValue();
	filterType = guiControls->filtTypeKnob->GetValue();



}