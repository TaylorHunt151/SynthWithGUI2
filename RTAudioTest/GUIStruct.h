#pragma once
#include <wx/wx.h>
#include "GUIClasses.h"

struct GUIControls {

	/**********************************
	* OSCILLATOR 1 CONTROLS
	***********************************/
	wxChoice* oscTypeMenu = nullptr;
	KnobControl* oscVolKnob = nullptr;
	KnobControl* oscPhsKnob = nullptr;
	KnobControl* oscPitchKnob = nullptr;

	/**********************************
	* OSCILLATOR 2 CONTROLS
	***********************************/
	wxChoice* osc2TypeMenu = nullptr;
	KnobControl* osc2VolKnob = nullptr;
	KnobControl* osc2PhsKnob = nullptr;
	KnobControl* osc2PitchKnob = nullptr;

	/**********************************
	* ENVELOPE CONTROLS
	***********************************/
	KnobControl* envAtk = nullptr;
	KnobControl* envDec = nullptr;
	KnobControl* envSus = nullptr;
	KnobControl* envRel = nullptr;

	/**********************************
	* FILTER CONTROLS
	***********************************/
	wxChoice* filtOnChoice = nullptr;
	wxChoice* filtKeyTrack = nullptr;
	KnobControl* cutoffKnob = nullptr;
	KnobControl* filtQKnob = nullptr;
	KnobControl* filtTypeKnob = nullptr;

	/**********************************
	* FLANGER CONTROLS
	***********************************/
	wxChoice* flngOn = nullptr;
	KnobControl* flngTime = nullptr;
	KnobControl* flngWet = nullptr;
	KnobControl* flngFdbck = nullptr;
	KnobControl* flngModAmnt = nullptr;
	KnobControl* flngModFreq = nullptr;
	KnobControl* flngCutoff = nullptr;
	KnobControl* flngQ = nullptr;
	KnobControl* flngFilterType = nullptr;

	/**********************************
	* CHORUS CONTROLS
	***********************************/
	wxChoice* chrsOn = nullptr;
	KnobControl* chrsTime = nullptr;
	KnobControl* chrsWet = nullptr;
	KnobControl* chrsFdbck = nullptr;
	KnobControl* chrsModAmnt = nullptr;
	KnobControl* chrsModFreq = nullptr;
	KnobControl* chrsCutoff = nullptr;
	KnobControl* chrsQ = nullptr;
	KnobControl* chrsFilterType = nullptr;

	/**********************************
	* DELAY CONTROLS
	***********************************/
	wxChoice* delayOnCheck = nullptr;
	wxChoice* dlyDistTypeMenu = nullptr;
	KnobControl* delayTimeKnob = nullptr;
	KnobControl* delayWetKnob = nullptr;
	KnobControl* delayFeedback = nullptr;
	KnobControl* delayCutoff = nullptr;
	KnobControl* delayQ = nullptr;
	KnobControl* delayFiltType = nullptr;
	KnobControl* delayDrive = nullptr;
	KnobControl* delayDistMix = nullptr;

	/**********************************
	* REVERB CONTROLS
	***********************************/
	wxChoice* verbOnCheck = nullptr;
	KnobControl* verbDecay = nullptr;
	KnobControl* verbWet = nullptr;
	KnobControl* verbDamp = nullptr;
	KnobControl* verbPreDly = nullptr;
	KnobControl* verbFiltType = nullptr;
	KnobControl* verbFiltQ = nullptr;
	KnobControl* verbCutoff = nullptr;
	KnobControl* verbModFreq = nullptr;
	KnobControl* verbModAmp = nullptr;

	/**********************************
	* DISTORTION CONTROLS
	***********************************/
	wxChoice* distType = nullptr;
	KnobControl* distWet = nullptr;
	KnobControl* distDrive = nullptr;
	KnobControl* distCutoff = nullptr;
	KnobControl* distQ = nullptr;
	KnobControl* distFiltType = nullptr;

};