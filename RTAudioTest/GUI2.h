#pragma once
#include "GUIClasses.h"
#include "GUIStruct.h"

extern GUIControls guiControls;

void GUI2(wxFrame* window, wxPanel* panel, wxBoxSizer* mainSizer) {



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


	KnobControl* oscVolKnob = new KnobControl(panel, wxID_ANY, 0, 1);//...And so on for each variable
	oscVolKnob->SetValue(80);
	guiControls.oscVolKnob = oscVolKnob;
	
	KnobControl* oscPhsKnob = new KnobControl(panel, wxID_ANY, 0, 1);
	oscPhsKnob->SetValue(0);
	guiControls.oscPhsKnob = oscPhsKnob;


	KnobControl* oscPitchKnob = new KnobControl(panel, wxID_ANY, -100, 100);
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

	KnobControl* osc2VolKnob = new KnobControl(panel, wxID_ANY, 0, 1);
	osc2VolKnob->SetValue(80);
	guiControls.osc2VolKnob = osc2VolKnob;

	KnobControl* osc2PhsKnob = new KnobControl(panel, wxID_ANY, 0, 1);
	osc2PhsKnob->SetValue(0);
	guiControls.osc2PhsKnob = osc2PhsKnob;

	
	KnobControl* osc2PitchKnob = new KnobControl(panel, wxID_ANY, -100, 100);
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

	KnobControl* envAtk = new KnobControl(panel, wxID_ANY, 0, 20);
	envAtk->SetValue(1);
	guiControls.envAtk = envAtk;

	KnobControl* envDec = new KnobControl(panel, wxID_ANY, 0, 20);
	envDec->SetValue(1);
	guiControls.envDec = envDec;

	KnobControl* envSus = new KnobControl(panel, wxID_ANY, 0, 20);
	envSus->SetValue(1);
	guiControls.envSus = envSus;

	KnobControl* envRel = new KnobControl(panel, wxID_ANY, 0, 20);
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

;
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
	toggleStrKeyTrck.Add("Keytrack Off");
	toggleStrKeyTrck.Add("Keytrack On");

	wxChoice* filtKeyTrack = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStrKeyTrck);
	filtKeyTrack->SetSelection(0);
	guiControls.filtKeyTrack = filtKeyTrack;
	filtKeyTrack->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});

	KnobControl* cutoffKnob = new KnobControl(panel, wxID_ANY, 80, 18000);
	cutoffKnob->SetValue(880);
	guiControls.cutoffKnob = cutoffKnob;

	KnobControl* filtQKnob = new KnobControl(panel, wxID_ANY, 0, 10);
	filtQKnob->SetValue(1);
	guiControls.filtQKnob = filtQKnob;

	KnobControl* filtTypeKnob = new KnobControl(panel, wxID_ANY, -1, 1);
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

	/********************************************************************
	* FX RACK GUI
	*********************************************************************/

	wxBoxSizer* fxSizer = new wxBoxSizer(wxHORIZONTAL); //creates a new row for the fx rack

	/*************************************************************************
	* FLANGER GUI STUFF
	**************************************************************************/

	wxBoxSizer* flngBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBox* flngBox = new wxStaticBox(panel, wxID_ANY, "Flanger");
	wxGridSizer* flngGrid = new wxGridSizer(2, 5, 5, 5);

	wxChoice* flngOn = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStr);
	flngOn->SetSelection(0);
	flngOn->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	guiControls.flngOn = flngOn;

	KnobControl* flngTime = new KnobControl(panel, wxID_ANY, 0, 3);
	flngTime->SetValue(1);
	guiControls.flngTime = flngTime;

	KnobControl* flngWet = new KnobControl(panel, wxID_ANY, 0, 100);
	flngWet->SetValue(50);
	guiControls.flngWet = flngWet;

	KnobControl* flngFdbck = new KnobControl(panel, wxID_ANY, 0, 100);
	flngFdbck->SetValue(50);
	guiControls.flngFdbck = flngFdbck;

	KnobControl* flngModAmnt = new KnobControl(panel, wxID_ANY, 0, 3);
	flngModAmnt->SetValue(3);
	guiControls.flngModAmnt = flngModAmnt;

	KnobControl* flngModFreq = new KnobControl(panel, wxID_ANY, 0, 10);
	flngModFreq->SetValue(1);
	guiControls.flngModFreq = flngModFreq;

	KnobControl* flngCutoff = new KnobControl(panel, wxID_ANY, 80, 20000);
	flngCutoff->SetValue(440);
	guiControls.flngCutoff = flngCutoff;

	KnobControl* flngQ = new KnobControl(panel, wxID_ANY, 0, 10);
	flngQ->SetValue(1);
	guiControls.flngQ = flngQ;

	KnobControl* flngFilterType = new KnobControl(panel, wxID_ANY, -1, 1);
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
	voiceSizer->Add(flngBoxSizer, 0, wxALL, 10);

	
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

	KnobControl* chrsTime = new KnobControl(panel, wxID_ANY, 0, 3);
	chrsTime->SetValue(1);
	guiControls.chrsTime = chrsTime;

	KnobControl* chrsWet = new KnobControl(panel, wxID_ANY, 0, 100);
	chrsWet->SetValue(50);
	guiControls.chrsWet = chrsWet;

	KnobControl* chrsFdbck = new KnobControl(panel, wxID_ANY, 0, 100);
	chrsFdbck->SetValue(50);
	guiControls.chrsFdbck = chrsFdbck;

	KnobControl* chrsModAmnt = new KnobControl(panel, wxID_ANY, 0, 3);
	chrsModAmnt->SetValue(3);
	guiControls.chrsModAmnt = chrsModAmnt;

	KnobControl* chrsModFreq = new KnobControl(panel, wxID_ANY, 0, 10);
	chrsModFreq->SetValue(1);
	guiControls.chrsModFreq = chrsModFreq;

	KnobControl* chrsCutoff = new KnobControl(panel, wxID_ANY, 80, 20000);
	chrsCutoff->SetValue(440);
	guiControls.chrsCutoff = chrsCutoff;

	KnobControl* chrsQ = new KnobControl(panel, wxID_ANY, 0, 10);
	chrsQ->SetValue(1);
	guiControls.chrsQ = chrsQ;

	KnobControl* chrsFilterType = new KnobControl(panel, wxID_ANY, -1, 1);
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
	wxGridSizer* delayGrid = new wxGridSizer(3, 4, 5, 5);

	wxChoice* delayOnCheck = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, toggleStr);
	delayOnCheck->SetSelection(0);
	delayOnCheck->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	guiControls.delayOnCheck = delayOnCheck;

	KnobControl* delayTimeKnob = new KnobControl(panel, wxID_ANY, 0, 10);
	delayTimeKnob->SetValue(1);
	guiControls.delayTimeKnob = delayTimeKnob;

	KnobControl* delayWetKnob = new KnobControl(panel, wxID_ANY, 0, 100);
	delayWetKnob->SetValue(50);
	guiControls.delayWetKnob = delayWetKnob;

	KnobControl* delayFeedback = new KnobControl(panel, wxID_ANY, 0, 100);
	delayFeedback->SetValue(50);
	guiControls.delayFeedback = delayFeedback;

	KnobControl* delayCutoff = new KnobControl(panel, wxID_ANY, 80, 18000);
	delayCutoff->SetValue(440);
	guiControls.delayCutoff = delayCutoff;

	KnobControl* delayFiltQ = new KnobControl(panel, wxID_ANY, 0, 10);
	delayFiltQ->SetValue(1);
	guiControls.delayFiltQ = delayFiltQ;

	KnobControl* delayFiltType = new KnobControl(panel, wxID_ANY, -1, 1);
	delayFiltType->SetValue(0);
	guiControls.delayFiltType = delayFiltType;

	KnobControl* delayDrive = new KnobControl(panel, wxID_ANY, 0, 100);
	delayDrive->SetValue(10);
	guiControls.delayDrive = delayDrive;

	KnobControl* delayDistMix = new KnobControl(panel, wxID_ANY, 0, 100);	
	delayDistMix->SetValue(25);
	guiControls.delayDistMix = delayDistMix;

	wxArrayString dlyDistTypeStr;
	dlyDistTypeStr.Add("1");
	dlyDistTypeStr.Add("2");
	dlyDistTypeStr.Add("3");
	dlyDistTypeStr.Add("4");
	dlyDistTypeStr.Add("5");
	dlyDistTypeStr.Add("6");
	dlyDistTypeStr.Add("Distort Off");

	wxChoice* dlyDistTypeMenu = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, dlyDistTypeStr);
	dlyDistTypeMenu->SetSelection(6);
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
	delayGrid->Add(delayDrive, 0, wxEXPAND);
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

	KnobControl* verbDecay = new KnobControl(panel, wxID_ANY, 0, 1);
	verbDecay->SetValue(1);
	guiControls.verbDecay = verbDecay;

	KnobControl* verbWet = new KnobControl(panel, wxID_ANY, 0, 100);
	verbWet->SetValue(25);
	guiControls.verbWet = verbWet;

	KnobControl* verbDamp = new KnobControl(panel, wxID_ANY, 80, 18000);
	verbDamp->SetValue(1000);
	guiControls.verbDamp = verbDamp;

	KnobControl* verbPreDly = new KnobControl(panel, wxID_ANY, 0, 3000);
	verbPreDly->SetValue(500);
	guiControls.verbPreDly = verbPreDly;

	KnobControl* verbFiltType = new KnobControl(panel, wxID_ANY, -1, 1);
	verbFiltType->SetValue(1);
	guiControls.verbFiltType = verbFiltType;

	KnobControl* verbFiltQ = new KnobControl(panel, wxID_ANY, 0, 10);
	verbFiltQ->SetValue(1);
	guiControls.verbFiltQ = verbFiltQ;

	KnobControl* verbCutoff = new KnobControl(panel, wxID_ANY, 80, 18000);
	verbCutoff->SetValue(2000);
	guiControls.verbCutoff = verbCutoff;

	KnobControl* verbModFreq = new KnobControl(panel, wxID_ANY, 0, 10);
	verbModFreq->SetValue(1);
	guiControls.verbModFreq = verbModFreq;

	KnobControl* verbModAmp = new KnobControl(panel, wxID_ANY, 0, 128);
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
	distType->SetSelection(6);
	distType->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
		event.Skip();
		window->SetFocus();
		});
	KnobControl* distWet = new KnobControl(panel, wxID_ANY, 0, 100);
	distWet->SetValue(50);
	KnobControl* distDrive = new KnobControl(panel, wxID_ANY, 0, 100);
	distDrive->SetValue(2);
	KnobControl* distCutoff = new KnobControl(panel, wxID_ANY, 80, 18000);
	distCutoff->SetValue(440);
	KnobControl* distQ = new KnobControl(panel, wxID_ANY, 0, 10);
	distQ->SetValue(1);
	KnobControl* distFiltType = new KnobControl(panel, wxID_ANY, -1, 1);
	distFiltType->SetValue(0);
	

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

