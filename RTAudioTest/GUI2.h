#pragma once
#include "GUIClasses.h"
#include "GUIStruct.h"

extern GUIControls guiControls;

void GUI2(wxFrame* window, wxPanel* panel, wxBoxSizer* mainSizer) { //This handles a large bulk of the GUI fron-end code



	wxBoxSizer* voiceSizer = new wxBoxSizer(wxHORIZONTAL);

	/***********************************************************************************
	* OSCILLATOR 1 KNOBS
	************************************************************************************/

	wxBoxSizer* oscBoxSizer = new wxBoxSizer(wxHORIZONTAL); //Box for oscillator knobs

	wxStaticBox* oscBox = new wxStaticBox(panel, wxID_ANY, "Oscillator 1");
	wxGridSizer* oscGrid = new wxGridSizer(2, 2, 5, 5);

	wxArrayString oscTypeStr;
	oscTypeStr.Add("Sine");
	oscTypeStr.Add("Square");
	oscTypeStr.Add("Saw");
	oscTypeStr.Add("Triangle");
	oscTypeStr.Add("Noise");//Adds the options for oscillator types to a string that will be used by the wxChoice object to determine its choices
	oscTypeStr.Add("Off");

	wxChoice* oscTypeMenu = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, oscTypeStr);//Creates a pointer that points to a new GUI Object 
	oscTypeMenu->SetSelection(5);//Sets default value
	guiControls.oscTypeMenu = oscTypeMenu;//Adds the GUI Object pointer to a global struct which allows it to be accessed from outside the function

	oscTypeMenu->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {//For wxChoice menus, it it necessary to shift the application's focus back to the window after selecting an option. This is because, by default, wxChoice objects will grab your keyboard input when you click them and it causes all kinds of havoc. This gets rid of that possibility.
		event.Skip();
		window->SetFocus();
		});


	KnobControl* oscVolKnob = new KnobControl(panel, wxID_ANY, 0, 0.1, ScaleType::Exponential, "Volume");//...And so on for each variable
	oscVolKnob->SetValue(80);
	guiControls.oscVolKnob = oscVolKnob;
	
	KnobControl* oscPhsKnob = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Phase");
	oscPhsKnob->SetValue(0);
	guiControls.oscPhsKnob = oscPhsKnob;


	KnobControl* oscPitchKnob = new KnobControl(panel, wxID_ANY, -0.5, 0.5, ScaleType::Linear, "PitchShift");
	oscPitchKnob->SetValue(0);
	guiControls.oscPitchKnob = oscPitchKnob;

	
	oscGrid->Add(oscTypeMenu, 0);//Add the object to the grid
	oscGrid->Add(oscVolKnob, 0, wxEXPAND);//...and so on for each variable
	oscGrid->Add(oscPhsKnob, 0, wxEXPAND);
	oscGrid->Add(oscPitchKnob, 0, wxEXPAND);
	

	wxStaticBoxSizer* oscBoxSizerStatic = new wxStaticBoxSizer(oscBox, wxVERTICAL);

	oscBoxSizerStatic->Add(oscGrid, 0, wxALL, 10);
	oscBoxSizer->Add(oscBoxSizerStatic, 0, wxALL, 10);
	voiceSizer->Add(oscBoxSizer, 0, wxEXPAND);



	/***********************************************************************************
	* OSCILLATOR 2 KNOBS
	************************************************************************************/

	wxBoxSizer* osc2BoxSizer = new wxBoxSizer(wxHORIZONTAL); //Box for oscillator 2 knobs

	wxStaticBox* osc2Box = new wxStaticBox(panel, wxID_ANY, "Oscillator 2");
	wxGridSizer* osc2Grid = new wxGridSizer(2, 2, 5, 5);


	wxChoice* osc2TypeMenu = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, oscTypeStr);
	osc2TypeMenu->SetSelection(5);//Sets default value to "Off"
	guiControls.osc2TypeMenu = osc2TypeMenu;
	osc2TypeMenu->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});

	KnobControl* osc2VolKnob = new KnobControl(panel, wxID_ANY, 0, 0.1, ScaleType::Exponential, "Volume");
	osc2VolKnob->SetValue(80);
	guiControls.osc2VolKnob = osc2VolKnob;

	KnobControl* osc2PhsKnob = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Phase");
	osc2PhsKnob->SetValue(0);
	guiControls.osc2PhsKnob = osc2PhsKnob;

	
	KnobControl* osc2PitchKnob = new KnobControl(panel, wxID_ANY, -0.5, 0.5, ScaleType::Linear, "PitchShift");
	osc2PitchKnob->SetValue(0);
	guiControls.osc2PitchKnob = osc2PitchKnob;

	
	osc2Grid->Add(osc2TypeMenu, 0);
	osc2Grid->Add(osc2VolKnob, 0, wxEXPAND);
	osc2Grid->Add(osc2PhsKnob, 0, wxEXPAND);
	osc2Grid->Add(osc2PitchKnob, 0, wxEXPAND);
	

	wxStaticBoxSizer* osc2BoxSizerStatic = new wxStaticBoxSizer(osc2Box, wxVERTICAL);

	osc2BoxSizerStatic->Add(osc2Grid, 0, wxALL, 10);
	osc2BoxSizer->Add(osc2BoxSizerStatic, 0, wxALL, 10);
	voiceSizer->Add(osc2BoxSizer, 0, wxEXPAND);


	/***********************************************************************************
	* ENVELOPE KNOBS
	************************************************************************************/

	wxBoxSizer* envBoxSizer = new wxBoxSizer(wxHORIZONTAL); //Box for envelope knobs

	wxStaticBox* envBox = new wxStaticBox(panel, wxID_ANY, "Envelope/ADSR");
	wxGridSizer* envGrid = new wxGridSizer(2, 2, 5, 5);

	KnobControl* envAtk = new KnobControl(panel, wxID_ANY, 0.01, 20, ScaleType::Exponential, "Attack");
	envAtk->SetValue(1);
	guiControls.envAtk = envAtk;

	KnobControl* envDec = new KnobControl(panel, wxID_ANY, 0.01, 20, ScaleType::Exponential, "Decay");
	envDec->SetValue(1);
	guiControls.envDec = envDec;

	KnobControl* envSus = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Logarithmic, "Sustain");
	envSus->SetValue(1);
	guiControls.envSus = envSus;

	KnobControl* envRel = new KnobControl(panel, wxID_ANY, 0.01, 20, ScaleType::Exponential, "Release");
	envRel->SetValue(1);
	guiControls.envRel = envRel;


	envGrid->Add(envAtk, 0, wxEXPAND);
	envGrid->Add(envDec, 0, wxEXPAND);
	envGrid->Add(envSus, 0, wxEXPAND);
	envGrid->Add(envRel, 0, wxEXPAND);

	wxStaticBoxSizer* envStatSizer = new wxStaticBoxSizer(envBox, wxVERTICAL);

	envStatSizer->Add(envGrid, 0, wxALL, 10);
	envBoxSizer->Add(envStatSizer, 0, wxALL, 10);
	voiceSizer->Add(envBoxSizer, 0, wxEXPAND);
	

	/***********************************************************************************
	* FILTER KNOBS
	************************************************************************************/
	
	wxBoxSizer* filtBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* filtBox = new wxStaticBox(panel, wxID_ANY, "Filter");
	wxGridSizer* filtGrid = new wxGridSizer(2, 3, 5, 5);
	wxArrayString toggleStr;
	toggleStr.Add("Off");
	toggleStr.Add("On");

	wxChoice* filtOnChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStr);
	filtOnChoice->SetSelection(0);
	guiControls.filtOnChoice = filtOnChoice;
	filtOnChoice->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	wxArrayString toggleStrKeyTrck;
	toggleStrKeyTrck.Add("KeytrckOff");
	toggleStrKeyTrck.Add("KeytrckOn");
	wxChoice* filtKeyTrack = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStrKeyTrck);
	filtKeyTrack->SetSelection(0);
	guiControls.filtKeyTrack = filtKeyTrack;
	filtKeyTrack->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	KnobControl* cutoffKnob = new KnobControl(panel, wxID_ANY, 80, 18000, ScaleType::Exponential, "Cutoff");
	cutoffKnob->SetValue(220);
	guiControls.cutoffKnob = cutoffKnob;
	KnobControl* filtQKnob = new KnobControl(panel, wxID_ANY, 0.1, 100, ScaleType::Exponential, "Q");
	filtQKnob->SetValue(1);
	guiControls.filtQKnob = filtQKnob;
	KnobControl* filtTypeKnob = new KnobControl(panel, wxID_ANY, -0.9, 1, ScaleType::Linear, "Type");
	filtTypeKnob->SetValue(1);
	guiControls.filtTypeKnob = filtTypeKnob;

	filtGrid->Add(filtOnChoice, 0);
	filtGrid->Add(filtKeyTrack, 0);
	filtGrid->Add(cutoffKnob, 0, wxEXPAND);
	filtGrid->Add(filtQKnob, 0, wxEXPAND);
	filtGrid->Add(filtTypeKnob, 0, wxEXPAND);

	wxStaticBoxSizer* filtStatSizer = new wxStaticBoxSizer(filtBox, wxVERTICAL);

	filtStatSizer->Add(filtGrid, 0, wxALL, 10);
	filtBoxSizer->Add(filtStatSizer, 0, wxALL, 10);
	voiceSizer->Add(filtBoxSizer, 0, wxEXPAND);


	mainSizer->Add(voiceSizer, 0, wxEXPAND);//adds voice sizer to the mainsizer

	/***********************************************************************************
	* MODULATOR 1 KNOBS
	************************************************************************************/

	wxBoxSizer* modBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* modBox = new wxStaticBox(panel, wxID_ANY, "LFO1");
	wxGridSizer* modGrid = new wxGridSizer(2, 3, 5, 5);
	
	wxArrayString modTypeStr;
	modTypeStr.Add("Sine");
	modTypeStr.Add("Square");
	modTypeStr.Add("Saw");
	modTypeStr.Add("Triangle");

	wxChoice* modType = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, modTypeStr);
	modType->SetSelection(3);
	guiControls.modType = modType;

	wxArrayString modCarrierStr;
	modCarrierStr.Add("None");
	modCarrierStr.Add("Osc1Freq");
	modCarrierStr.Add("Osc2Freq");
	wxChoice* modCarrier = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, modCarrierStr);
	modCarrier->SetSelection(0);
	guiControls.modCarrier = modCarrier;
	KnobControl* modFreq = new KnobControl(panel, wxID_ANY, 0.01, 10, ScaleType::Exponential, "Freq");
	modFreq->SetValue(0.5);
	guiControls.modFreq = modFreq;
	KnobControl* modPhaseOff = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Phase");
	modPhaseOff->SetValue(0);
	guiControls.modPhase = modPhaseOff;
	KnobControl* modAmp = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Logarithmic, "Amp");
	guiControls.modAmp = modAmp;

	modGrid->Add(modCarrier, 0);
	modGrid->Add(modType, 0);
	modGrid->Add(modFreq, 0, wxEXPAND);
	modGrid->Add(modPhaseOff, 0, wxEXPAND);
	modGrid->Add(modAmp, 0, wxEXPAND);

	wxStaticBoxSizer* modStatSizer = new wxStaticBoxSizer(modBox, wxVERTICAL);

	modStatSizer->Add(modGrid, 0, wxALL, 10);
	modBoxSizer->Add(modStatSizer, 0, wxALL, 10);
	voiceSizer->Add(modBoxSizer, 0, wxEXPAND);

	/***********************************************************************************
	* MODULATOR 2 KNOBS
	************************************************************************************/

	wxBoxSizer* mod2BoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* mod2Box = new wxStaticBox(panel, wxID_ANY, "LFO2");
	wxGridSizer* mod2Grid = new wxGridSizer(2, 3, 5, 5);

	wxChoice* mod2Type = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, modTypeStr);
	mod2Type->SetSelection(3);
	guiControls.mod2Type = mod2Type;

	wxArrayString mod2CarrierStr;
	mod2CarrierStr.Add("None");
	mod2CarrierStr.Add("Osc1Freq");
	mod2CarrierStr.Add("Osc2Freq");
	wxChoice* mod2Carrier = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, mod2CarrierStr);
	mod2Carrier->SetSelection(0);
	guiControls.mod2Carrier = mod2Carrier;
	KnobControl* mod2Freq = new KnobControl(panel, wxID_ANY, 0.01, 10, ScaleType::Exponential, "Freq");
	mod2Freq->SetValue(0.5);
	guiControls.mod2Freq = mod2Freq;
	KnobControl* mod2PhaseOff = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Phase");
	mod2PhaseOff->SetValue(0);
	guiControls.mod2Phase = mod2PhaseOff;
	KnobControl* mod2Amp = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Logarithmic, "Amp");
	guiControls.mod2Amp = mod2Amp;

	mod2Grid->Add(mod2Carrier, 0);
	mod2Grid->Add(mod2Type, 0);
	mod2Grid->Add(mod2Freq, 0, wxEXPAND);
	mod2Grid->Add(mod2PhaseOff, 0, wxEXPAND);
	mod2Grid->Add(mod2Amp, 0, wxEXPAND);

	wxStaticBoxSizer* mod2StatSizer = new wxStaticBoxSizer(mod2Box, wxVERTICAL);

	mod2StatSizer->Add(mod2Grid, 0, wxALL, 10);
	mod2BoxSizer->Add(mod2StatSizer, 0, wxALL, 10);
	voiceSizer->Add(mod2BoxSizer, 0, wxEXPAND);


	/********************************************************************
	* FX RACK GUI
	*********************************************************************/

	wxBoxSizer* fxSizer = new wxBoxSizer(wxHORIZONTAL); //creates a new row for the fx rack




	/*************************************************************************
	* FLANGER GUI STUFF
	**************************************************************************/

	wxBoxSizer* flngBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* flngBox = new wxStaticBox(panel, wxID_ANY, "Flanger");
	wxGridSizer* flngGrid = new wxGridSizer(3, 3, 5, 5);

	wxChoice* flngOn = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStr);
	flngOn->SetSelection(0);
	flngOn->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	guiControls.flngOn = flngOn;

	KnobControl* flngTime = new KnobControl(panel, wxID_ANY, .0005, .005, ScaleType::Exponential, "Time");
	flngTime->SetValue(.001);
	guiControls.flngTime = flngTime;

	KnobControl* flngWet = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Mix");
	flngWet->SetValue(0.5);
	guiControls.flngWet = flngWet;

	KnobControl* flngFdbck = new KnobControl(panel, wxID_ANY, 0, 0.9, ScaleType::Linear, "Feedback");
	flngFdbck->SetValue(.4);
	guiControls.flngFdbck = flngFdbck;

	KnobControl* flngModAmnt = new KnobControl(panel, wxID_ANY, 0, 0.003, ScaleType::Exponential, "Mod Depth");
	flngModAmnt->SetValue(.002);
	guiControls.flngModAmnt = flngModAmnt;

	KnobControl* flngModFreq = new KnobControl(panel, wxID_ANY, 0.01, 10, ScaleType::Exponential, "Mod Freq");
	flngModFreq->SetValue(0.5);
	guiControls.flngModFreq = flngModFreq;

	KnobControl* flngCutoff = new KnobControl(panel, wxID_ANY, 80, 20000, ScaleType::Exponential, "Cutoff");
	flngCutoff->SetValue(440);
	guiControls.flngCutoff = flngCutoff;

	KnobControl* flngQ = new KnobControl(panel, wxID_ANY, 0.1, 10, ScaleType::Exponential, "Q");
	flngQ->SetValue(1);
	guiControls.flngQ = flngQ;

	KnobControl* flngFilterType = new KnobControl(panel, wxID_ANY, -1, 1, ScaleType::Linear, "Filter Type");
	flngFilterType->SetValue(0);
	guiControls.flngFilterType = flngFilterType;


	flngGrid->Add(flngOn, 0);
	flngGrid->Add(flngTime, 0, wxEXPAND);
	flngGrid->Add(flngWet, 0, wxEXPAND);
	flngGrid->Add(flngFdbck, 0, wxEXPAND);
	flngGrid->Add(flngModAmnt, 0, wxEXPAND);
	flngGrid->Add(flngModFreq, 0, wxEXPAND);
	flngGrid->Add(flngCutoff, 0, wxEXPAND);
	flngGrid->Add(flngQ, 0, wxEXPAND);
	flngGrid->Add(flngFilterType, 0, wxEXPAND);

	wxStaticBoxSizer* flngStatSizer = new wxStaticBoxSizer(flngBox, wxVERTICAL);

	flngStatSizer->Add(flngGrid, 0, wxALL, 10);
	flngBoxSizer->Add(flngStatSizer, 0, wxALL, 10);
	fxSizer->Add(flngBoxSizer, 0, wxALL, 10);

	
	/********************************************************************
	* CHORUS GUI
	*********************************************************************/

	wxBoxSizer* chrsBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* chrsBox = new wxStaticBox(panel, wxID_ANY, "Chorus");
	wxGridSizer* chrsGrid = new wxGridSizer(3, 3, 5, 5);

	wxChoice* chrsOn = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStr);
	chrsOn->SetSelection(0);
	chrsOn->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	guiControls.chrsOn = chrsOn;

	KnobControl* chrsTime = new KnobControl(panel, wxID_ANY, 0.003, .05, ScaleType::Exponential, "Time");
	chrsTime->SetValue(.03);
	guiControls.chrsTime = chrsTime;

	KnobControl* chrsWet = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Mix");
	chrsWet->SetValue(0.5);
	guiControls.chrsWet = chrsWet;

	KnobControl* chrsFdbck = new KnobControl(panel, wxID_ANY, 0, 0.99, ScaleType::Linear, "Feedback");
	chrsFdbck->SetValue(0.5);
	guiControls.chrsFdbck = chrsFdbck;

	KnobControl* chrsModAmnt = new KnobControl(panel, wxID_ANY, 0, 0.03, ScaleType::Exponential, "Mod Depth");
	chrsModAmnt->SetValue(0.002);
	guiControls.chrsModAmnt = chrsModAmnt;

	KnobControl* chrsModFreq = new KnobControl(panel, wxID_ANY, 0.01, 10, ScaleType::Exponential, "Mod Freq");
	chrsModFreq->SetValue(0.5);
	guiControls.chrsModFreq = chrsModFreq;

	KnobControl* chrsCutoff = new KnobControl(panel, wxID_ANY, 80, 20000, ScaleType::Exponential, "Cutoff");
	chrsCutoff->SetValue(440);
	guiControls.chrsCutoff = chrsCutoff;

	KnobControl* chrsQ = new KnobControl(panel, wxID_ANY, 0.1, 10, ScaleType::Exponential, "Q");
	chrsQ->SetValue(1);
	guiControls.chrsQ = chrsQ;

	KnobControl* chrsFilterType = new KnobControl(panel, wxID_ANY, -1, 1, ScaleType::Linear, "Filter Type");
	chrsFilterType->SetValue(0);
	guiControls.chrsFilterType = chrsFilterType;


	chrsGrid->Add(chrsOn, 0);
	chrsGrid->Add(chrsTime, 0, wxEXPAND);
	chrsGrid->Add(chrsWet, 0, wxEXPAND);
	chrsGrid->Add(chrsFdbck, 0, wxEXPAND);
	chrsGrid->Add(chrsModAmnt, 0, wxEXPAND);
	chrsGrid->Add(chrsModFreq, 0, wxEXPAND);
	chrsGrid->Add(chrsCutoff, 0, wxEXPAND);
	chrsGrid->Add(chrsQ, 0, wxEXPAND);
	chrsGrid->Add(chrsFilterType, 0, wxEXPAND);

	wxStaticBoxSizer* chrsStatSizer = new wxStaticBoxSizer(chrsBox, wxVERTICAL);

	chrsStatSizer->Add(chrsGrid, 0, wxALL, 10);
	chrsBoxSizer->Add(chrsStatSizer, 0, wxALL, 10);
	fxSizer->Add(chrsBoxSizer, 0, wxALL, 10);

	/********************************************************************
	* DELAY GUI
	*********************************************************************/
	wxBoxSizer* delayBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* delayBox = new wxStaticBox(panel, wxID_ANY, "Delay");
	wxGridSizer* delayGrid = new wxGridSizer(3, 3, 5, 5);

	wxChoice* delayOnCheck = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStr);
	delayOnCheck->SetSelection(0);
	delayOnCheck->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	guiControls.delayOnCheck = delayOnCheck;

	KnobControl* delayTimeKnob = new KnobControl(panel, wxID_ANY, 0.1, 5, ScaleType::Exponential, "Time");
	delayTimeKnob->SetValue(0.25);
	guiControls.delayTimeKnob = delayTimeKnob;

	KnobControl* delayWetKnob = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Mix");
	delayWetKnob->SetValue(0.5);
	guiControls.delayWetKnob = delayWetKnob;

	KnobControl* delayFeedback = new KnobControl(panel, wxID_ANY, 0, 0.9, ScaleType::Exponential, "Feedback");
	delayFeedback->SetValue(0.5);
	guiControls.delayFeedback = delayFeedback;

	KnobControl* delayCutoff = new KnobControl(panel, wxID_ANY, 80, 18000, ScaleType::Exponential, "Cutoff");
	delayCutoff->SetValue(440);
	guiControls.delayCutoff = delayCutoff;

	KnobControl* delayFiltQ = new KnobControl(panel, wxID_ANY, 0.1, 10, ScaleType::Exponential, "Q");
	delayFiltQ->SetValue(1);
	guiControls.delayQ = delayFiltQ;

	KnobControl* delayFiltType = new KnobControl(panel, wxID_ANY, -1, 1, ScaleType::Linear, "Filter Type");
	delayFiltType->SetValue(0);
	guiControls.delayFiltType = delayFiltType;

	//KnobControl* delayDrive = new KnobControl(panel, wxID_ANY, 0.1, 10, ScaleType::Logarithmic, "Drive");
	//delayDrive->SetValue(1);
	//guiControls.delayDrive = delayDrive;

	KnobControl* delayDistMix = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Dist Mix");
	delayDistMix->SetValue(0.5);
	guiControls.delayDistMix = delayDistMix;

	wxArrayString dlyDistTypeStr;
	dlyDistTypeStr.Add("1");
	dlyDistTypeStr.Add("2");
	dlyDistTypeStr.Add("3");
	dlyDistTypeStr.Add("4");
	dlyDistTypeStr.Add("5");
	dlyDistTypeStr.Add("Distort Off");

	wxChoice* dlyDistTypeMenu = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, dlyDistTypeStr);
	dlyDistTypeMenu->SetSelection(5);
	dlyDistTypeMenu->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	guiControls.dlyDistTypeMenu = dlyDistTypeMenu;

	delayGrid->Add(delayOnCheck, 0);
	delayGrid->Add(delayTimeKnob, 0, wxEXPAND);
	delayGrid->Add(delayWetKnob, 0, wxEXPAND);
	delayGrid->Add(delayFeedback, 0, wxEXPAND);
	delayGrid->Add(delayCutoff, 0, wxEXPAND);
	delayGrid->Add(delayFiltQ, 0, wxEXPAND);
	delayGrid->Add(delayFiltType, 0, wxEXPAND);
	delayGrid->Add(dlyDistTypeMenu, 0);
	//delayGrid->Add(delayDrive, 0, wxEXPAND);
	delayGrid->Add(delayDistMix, 0, wxEXPAND);

	wxStaticBoxSizer* delayStatSizer = new wxStaticBoxSizer(delayBox, wxVERTICAL);

	delayStatSizer->Add(delayGrid, 0, wxALL, 10);
	delayBoxSizer->Add(delayStatSizer, 0, wxALL, 10);
	fxSizer->Add(delayBoxSizer, 0, wxEXPAND);

	/********************************************************************
	* GOODVERB GUI
	*********************************************************************/

	wxBoxSizer* verbBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* verbBox = new wxStaticBox(panel, wxID_ANY, "Plate Reverb");
	wxGridSizer* verbGrid = new wxGridSizer(3, 4, 5, 5);

	wxChoice* verbOnCheck = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStr);
	verbOnCheck->SetSelection(0);
	verbOnCheck->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	guiControls.verbOnCheck = verbOnCheck;

	KnobControl* verbDecay = new KnobControl(panel, wxID_ANY, 0.001, 0.4, ScaleType::Exponential, "Decay");
	verbDecay->SetValue(0.1);
	guiControls.verbDecay = verbDecay;

	KnobControl* verbWet = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Mix");
	verbWet->SetValue(0.5);
	guiControls.verbWet = verbWet;

	KnobControl* verbDamp = new KnobControl(panel, wxID_ANY, 80, 15000, ScaleType::Exponential, "DampCut");
	verbDamp->SetValue(1000);
	guiControls.verbDamp = verbDamp;

	KnobControl* verbPreDly = new KnobControl(panel, wxID_ANY, 0, 3000, ScaleType::Exponential, "PreDly");
	verbPreDly->SetValue(500);
	guiControls.verbPreDly = verbPreDly;

	KnobControl* verbFiltType = new KnobControl(panel, wxID_ANY, -1, 1, ScaleType::Linear, "Filter Type");
	verbFiltType->SetValue(1);
	guiControls.verbFiltType = verbFiltType;

	KnobControl* verbFiltQ = new KnobControl(panel, wxID_ANY, 0.1, 10, ScaleType::Exponential, "Q");
	verbFiltQ->SetValue(1);
	guiControls.verbFiltQ = verbFiltQ;

	KnobControl* verbCutoff = new KnobControl(panel, wxID_ANY, 80, 18000, ScaleType::Exponential, "Cutoff");
	verbCutoff->SetValue(2000);
	guiControls.verbCutoff = verbCutoff;

	KnobControl* verbModFreq = new KnobControl(panel, wxID_ANY, 0.01, 10, ScaleType::Exponential, "Mod Freq");
	verbModFreq->SetValue(0.5);
	guiControls.verbModFreq = verbModFreq;

	KnobControl* verbModAmp = new KnobControl(panel, wxID_ANY, 0, 128, ScaleType::Exponential, "Mod Amp");
	verbModAmp->SetValue(12);
	guiControls.verbModAmp = verbModAmp;


	verbGrid->Add(verbOnCheck, 0);
	verbGrid->Add(verbDecay, 0, wxEXPAND);
	verbGrid->Add(verbWet, 0, wxEXPAND);
	verbGrid->Add(verbDamp, 0, wxEXPAND);
	verbGrid->Add(verbPreDly, 0, wxEXPAND);
	verbGrid->Add(verbFiltType, 0, wxEXPAND);
	verbGrid->Add(verbFiltQ, 0, wxEXPAND);
	verbGrid->Add(verbCutoff, 0, wxEXPAND);
	verbGrid->Add(verbModFreq, 0, wxEXPAND);
	verbGrid->Add(verbModAmp, 0, wxEXPAND);

	wxStaticBoxSizer* verbStatSizer = new wxStaticBoxSizer(verbBox, wxVERTICAL);
	
	verbStatSizer->Add(verbGrid, 0, wxALL, 10);
	verbBoxSizer->Add(verbStatSizer, 0, wxALL, 10);
	fxSizer->Add(verbBoxSizer, 0, wxEXPAND);


	/********************************************************************
	* DISTORTION GUI
	*********************************************************************/

	wxBoxSizer* distBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* distBox = new wxStaticBox(panel, wxID_ANY, "Distortion");
	wxGridSizer* distGrid = new wxGridSizer(3, 2, 5, 5);

	wxChoice* distType = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, dlyDistTypeStr);
	distType->SetSelection(5);
	distType->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	guiControls.distType = distType;

	KnobControl* distWet = new KnobControl(panel, wxID_ANY, 0, 1, ScaleType::Linear, "Mix");
	distWet->SetValue(0.5);
	guiControls.distWet = distWet;

	KnobControl* distDrive = new KnobControl(panel, wxID_ANY, 0.01, 10, ScaleType::Linear, "Drive");
	distDrive->SetValue(2);
	guiControls.distDrive = distDrive;

	KnobControl* distCutoff = new KnobControl(panel, wxID_ANY, 80, 18000, ScaleType::Exponential, "Cutoff");
	distCutoff->SetValue(440);
	guiControls.distCutoff = distCutoff;

	KnobControl* distQ = new KnobControl(panel, wxID_ANY, 0.1, 10, ScaleType::Exponential, "Q");
	distQ->SetValue(1);
	guiControls.distQ = distQ;

	KnobControl* distFiltType = new KnobControl(panel, wxID_ANY, -1, 1, ScaleType::Linear, "FilterType");
	distFiltType->SetValue(0);
	guiControls.distFiltType = distFiltType;

	

	//distGrid->Add(distOnCheck, 0, wxEXPAND);
	distGrid->Add(distType, 0);
	distGrid->Add(distWet, 0, wxEXPAND);
	distGrid->Add(distDrive, 0, wxEXPAND);
	distGrid->Add(distCutoff, 0, wxEXPAND);
	distGrid->Add(distQ, 0, wxEXPAND);
	distGrid->Add(distFiltType, 0, wxEXPAND);

	wxStaticBoxSizer* distStatSizer = new wxStaticBoxSizer(distBox, wxVERTICAL);

	distStatSizer->Add(distGrid, 0, wxALL, 10);
	distBoxSizer->Add(distStatSizer, 0, wxALL, 10);
	fxSizer->Add(distBoxSizer, 0, wxEXPAND);

	mainSizer->Add(fxSizer, 0, wxEXPAND);//adds voice sizer to the mainsizer
}

