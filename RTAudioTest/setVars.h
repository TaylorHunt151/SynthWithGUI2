#pragma once

#include "VoiceFunctions.h"
#include "GUIStruct.h"

void voice::setVars(GUIControls* guiControls) { //This sets the parameters of all the voices

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

void setFXVars(GUIControls* guiControls, Flanger* flanger, Chorus* chorus, Delay* dly, Goodverb* verb, Distortion* dist) {//This sets the parameters of all the effects on the fx rack

	//FLANGER
	flanger->on = guiControls->flngOn->GetSelection();
	flanger->delayTime = guiControls->flngTime->GetValue();
	flanger->wetMix = guiControls->flngWet->GetValue();
	flanger->feedback = guiControls->flngFdbck->GetValue();
	flanger->modAmnt = guiControls->flngModAmnt->GetValue();
	flanger->modFreq = guiControls->flngModFreq->GetValue();
	flanger->cutoffSet = guiControls->flngCutoff->GetValue();
	flanger->q = guiControls->flngQ->GetValue();
	flanger->filterType = guiControls->flngFilterType->GetValue();

	//CHORUS
	chorus->on = guiControls->chrsOn->GetSelection();
	chorus->delayTime = guiControls->chrsTime->GetValue();
	chorus->wetMix = guiControls->chrsWet->GetValue();
	chorus->feedback = guiControls->chrsFdbck->GetValue();
	chorus->modAmnt = guiControls->chrsModAmnt->GetValue();
	chorus->modFreq = guiControls->chrsModFreq->GetValue();
	chorus->cutoffSet = guiControls->chrsCutoff->GetValue();
	chorus->q = guiControls->chrsQ->GetValue();
	chorus->filterType = guiControls->chrsFilterType->GetValue();

	//DELAY
	dly->on = guiControls->delayOnCheck->GetSelection();
	dly->delayTime = guiControls->delayTimeKnob->GetValue();
	dly->wetMix = guiControls->delayWetKnob->GetValue();
	dly->feedback = guiControls->delayFeedback->GetValue();
	dly->distType = guiControls->dlyDistTypeMenu->GetSelection();
	dly->distMix = guiControls->delayDistMix->GetValue();
	dly->drive = guiControls->delayDrive->GetValue();
	dly->cutoffSet = guiControls->delayCutoff->GetValue();
	dly->q = guiControls->delayQ->GetValue();
	dly->filterType = guiControls->delayFiltType->GetValue();


	//REVERB
	verb->on = guiControls->verbOnCheck->GetSelection();
	verb->decayTime = guiControls->verbDecay->GetValue();
	verb->wetMix = guiControls->verbWet->GetValue();
	verb->dampFreq = guiControls->verbDamp->GetValue();
	verb->preDelay = guiControls->verbPreDly->GetValue();
	verb->filterType = guiControls->verbFiltType->GetValue();
	verb->filtQ = guiControls->verbFiltQ->GetValue();
	verb->filterCutoff = guiControls->verbCutoff->GetValue();
	verb->modFreq = guiControls->verbModFreq->GetValue();
	verb->modAmp = guiControls->verbModAmp->GetValue();


	//DISTORTION
	dist->type = guiControls->distType->GetSelection();
	dist->wetMix = guiControls->distWet->GetValue();
	dist->drive = guiControls->distDrive->GetValue();
	dist->cutoffSet = guiControls->distCutoff->GetValue();
	dist->q = guiControls->distQ->GetValue();
	dist->filterType = guiControls->distFiltType->GetValue();

}