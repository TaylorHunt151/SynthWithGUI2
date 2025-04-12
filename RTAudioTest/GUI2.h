#pragma once
#include "GUIClasses.h"



void GUI2(wxFrame* window, wxPanel* panel, wxBoxSizer* mainSizer) {

	//wxImage gayfurrieskissing("C:\\Users\\taylo\\OneDrive\\Desktop\\gayfurrieskissing.png", wxBITMAP_TYPE_PNG); //society isn't ready for this feature


	wxBoxSizer* voiceSizer = new wxBoxSizer(wxHORIZONTAL);

	/***********************************************************************************
	* OSCILLATOR 1 KNOBS
	************************************************************************************/

	wxBoxSizer* oscBoxSizer = new wxBoxSizer(wxHORIZONTAL); //Box for oscillator knobs

	wxStaticBox* oscBox = new wxStaticBox(panel, wxID_ANY, "Oscillator 1");
	wxGridSizer* oscGrid = new wxGridSizer(2, 3, 5, 5);

	//wxCheckBox* checkbox = new wxCheckBox(parent, id, label, pos, size, style);
	wxCheckBox* oscOnButton = new wxCheckBox(panel, wxID_ANY, "On");
	oscOnButton->SetValue(0);//Add a checkbox to turn oscillator 1 on or off

	wxArrayString oscTypeStr;
	oscTypeStr.Add("Sine");
	oscTypeStr.Add("Square");
	oscTypeStr.Add("Saw");
	oscTypeStr.Add("Triangle");
	oscTypeStr.Add("Noise");//Adds the options for oscillator types to a string that will be used by the wxChoice object to determine its choices

	wxChoice* oscTypeMenu = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, oscTypeStr);
	oscTypeMenu->SetSelection(2);

	KnobControl* oscVolKnob = new KnobControl(panel, wxID_ANY, 0, 100);
	oscVolKnob->SetValue(80);
	
	KnobControl* oscPhsKnob = new KnobControl(panel, wxID_ANY, 0, 100);
	oscPhsKnob->SetValue(0);
	
	KnobControl* oscPitchKnob = new KnobControl(panel, wxID_ANY, -100, 100);
	oscPitchKnob->SetValue(0);
	
	oscGrid->Add(oscOnButton, 0, wxEXPAND);
	oscGrid->Add(oscTypeMenu, 0);
	oscGrid->Add(oscVolKnob, 0, wxEXPAND);
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
	wxGridSizer* osc2Grid = new wxGridSizer(2, 3, 5, 5);

	wxCheckBox* osc2OnButton = new wxCheckBox(panel, wxID_ANY, "On");
	osc2OnButton->SetValue(0);//Add a checkbox to turn oscillator 2 on or off

	wxArrayString osc2TypeStr;
	osc2TypeStr.Add("Sine");
	osc2TypeStr.Add("Square");
	osc2TypeStr.Add("Saw");
	osc2TypeStr.Add("Triangle");
	osc2TypeStr.Add("Noise");//Adds the options for oscillator types to a string that will be used by the wxChoice object to determine its choices

	wxChoice* osc2TypeMenu = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, osc2TypeStr);
	osc2TypeMenu->SetSelection(2);//Sets default value to "Saw"


	KnobControl* osc2VolKnob = new KnobControl(panel, wxID_ANY, 0, 100);
	osc2VolKnob->SetValue(80);
	
	KnobControl* osc2PhsKnob = new KnobControl(panel, wxID_ANY, 0, 100);
	osc2PhsKnob->SetValue(0);
	
	KnobControl* osc2PitchKnob = new KnobControl(panel, wxID_ANY, -100, 100);
	osc2PitchKnob->SetValue(0);
	
	osc2Grid->Add(osc2OnButton, 0, wxEXPAND);
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
	KnobControl* envDec = new KnobControl(panel, wxID_ANY, 0, 20);
	envDec->SetValue(1);
	KnobControl* envSus = new KnobControl(panel, wxID_ANY, 0, 20);
	envSus->SetValue(1);
	KnobControl* envRel = new KnobControl(panel, wxID_ANY, 0, 20);
	envRel->SetValue(1);

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

	wxCheckBox* filtOnCheck = new wxCheckBox(panel, wxID_ANY, "On");
	filtOnCheck->SetValue(0);
	wxCheckBox* filtKeyTrack = new wxCheckBox(panel, wxID_ANY, "Keytrack");
	filtKeyTrack->SetValue(0);
	KnobControl* cutoffKnob = new KnobControl(panel, wxID_ANY, 80, 18000);
	cutoffKnob->SetValue(880);
	KnobControl* filtQKnob = new KnobControl(panel, wxID_ANY, 0, 10);
	filtQKnob->SetValue(1);
	KnobControl* filtTypeKnob = new KnobControl(panel, wxID_ANY, -1, 1);


	filtGrid->Add(filtOnCheck, 0, wxEXPAND);
	filtGrid->Add(filtKeyTrack, 0, wxEXPAND);
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

	wxCheckBox* flngOn = new wxCheckBox(panel, wxID_ANY, "On");
	flngOn->SetValue(0);
	KnobControl* flngTime = new KnobControl(panel, wxID_ANY, 0, 3);
	flngTime->SetValue(1);
	KnobControl* flngWet = new KnobControl(panel, wxID_ANY, 0, 100);
	flngWet->SetValue(50);
	KnobControl* flngFdbck = new KnobControl(panel, wxID_ANY, 0, 100);
	flngFdbck->SetValue(50);
	KnobControl* flngModAmnt = new KnobControl(panel, wxID_ANY, 0, 3);
	flngModAmnt->SetValue(3);
	KnobControl* flngModFreq = new KnobControl(panel, wxID_ANY, 0, 10);
	flngModFreq->SetValue(1);
	KnobControl* flngCutoff = new KnobControl(panel, wxID_ANY, 80, 20000);
	flngCutoff->SetValue(440);
	KnobControl* flngQ = new KnobControl(panel, wxID_ANY, 0, 10);
	flngQ->SetValue(1);
	KnobControl* flngFilterType = new KnobControl(panel, wxID_ANY, -1, 1);
	flngFilterType->SetValue(0);

	flngGrid->Add(flngOn, 0, wxEXPAND);
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

	wxCheckBox* chrsOn = new wxCheckBox(panel, wxID_ANY, "On");
	chrsOn->SetValue(0);
	KnobControl* chrsTime = new KnobControl(panel, wxID_ANY, 0, 3);
	chrsTime->SetValue(1);
	KnobControl* chrsWet = new KnobControl(panel, wxID_ANY, 0, 100);
	chrsWet->SetValue(50);
	KnobControl* chrsFdbck = new KnobControl(panel, wxID_ANY, 0, 100);
	chrsFdbck->SetValue(50);
	KnobControl* chrsModAmnt = new KnobControl(panel, wxID_ANY, 0, 3);
	chrsModAmnt->SetValue(3);
	KnobControl* chrsModFreq = new KnobControl(panel, wxID_ANY, 0, 10);
	chrsModFreq->SetValue(1);
	KnobControl* chrsCutoff = new KnobControl(panel, wxID_ANY, 80, 20000);
	chrsCutoff->SetValue(440);
	KnobControl* chrsQ = new KnobControl(panel, wxID_ANY, 0, 10);
	chrsQ->SetValue(1);
	KnobControl* chrsFilterType = new KnobControl(panel, wxID_ANY, -1, 1);
	chrsFilterType->SetValue(0);

	chrsGrid->Add(chrsOn, 0, wxEXPAND);
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

	wxCheckBox* delayOnCheck = new wxCheckBox(panel, wxID_ANY, "On");
	delayOnCheck->SetValue(0);
	KnobControl* delayTimeKnob = new KnobControl(panel, wxID_ANY, 0, 10);
	delayTimeKnob->SetValue(1);
	KnobControl* delayWetKnob = new KnobControl(panel, wxID_ANY, 0, 100);
	delayWetKnob->SetValue(50);
	KnobControl* delayFeedback = new KnobControl(panel, wxID_ANY, 0, 100);
	delayFeedback->SetValue(50);
	KnobControl* delayCutoff = new KnobControl(panel, wxID_ANY, 80, 18000);
	delayCutoff->SetValue(440);
	KnobControl* delayFiltQ = new KnobControl(panel, wxID_ANY, 0, 10);
	delayFiltQ->SetValue(1);
	KnobControl* delayFiltType = new KnobControl(panel, wxID_ANY, -1, 1);
	delayFiltType->SetValue(0);
	KnobControl* delayDrive = new KnobControl(panel, wxID_ANY, 0, 100);
	delayDrive->SetValue(10);
	KnobControl* delayDistMix = new KnobControl(panel, wxID_ANY, 0, 100);	
	delayDistMix->SetValue(25);

	wxArrayString dlyDistTypeStr;
	dlyDistTypeStr.Add("1");
	dlyDistTypeStr.Add("2");
	dlyDistTypeStr.Add("3");
	dlyDistTypeStr.Add("4");
	dlyDistTypeStr.Add("5");
	dlyDistTypeStr.Add("6");

	wxChoice* dlyDistTypeMenu = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, dlyDistTypeStr);
	dlyDistTypeMenu->SetSelection(1);

	delayGrid->Add(delayOnCheck, 0, wxEXPAND);
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

	wxCheckBox* verbOnCheck = new wxCheckBox(panel, wxID_ANY, "On");
	verbOnCheck->SetValue(0);
	KnobControl* verbDecay = new KnobControl(panel, wxID_ANY, 0, 1);
	verbDecay->SetValue(1);
	KnobControl* verbWet = new KnobControl(panel, wxID_ANY, 0, 100);
	verbWet->SetValue(25);
	KnobControl* verbDamp = new KnobControl(panel, wxID_ANY, 80, 18000);
	verbDamp->SetValue(1000);
	KnobControl* verbPreDly = new KnobControl(panel, wxID_ANY, 0, 3000);
	verbPreDly->SetValue(500);
	KnobControl* verbFiltType = new KnobControl(panel, wxID_ANY, -1, 1);
	verbFiltType->SetValue(1);
	KnobControl* verbFiltQ = new KnobControl(panel, wxID_ANY, 0, 10);
	verbFiltQ->SetValue(1);
	KnobControl* verbCutoff = new KnobControl(panel, wxID_ANY, 80, 18000);
	verbCutoff->SetValue(2000);
	KnobControl* verbModFreq = new KnobControl(panel, wxID_ANY, 0, 10);
	verbModFreq->SetValue(1);
	KnobControl* verbModAmp = new KnobControl(panel, wxID_ANY, 0, 128);
	verbModAmp->SetValue(12);

	verbGrid->Add(verbOnCheck, 0, wxEXPAND);
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
	wxGridSizer* distGrid = new wxGridSizer(3, 3, 5, 5);

	wxCheckBox* distOnCheck = new wxCheckBox(panel, wxID_ANY, "On");
	distOnCheck->SetValue(0);
	wxChoice* distType = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, dlyDistTypeStr);
	distType->SetSelection(2);
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
	

	distGrid->Add(distOnCheck, 0, wxEXPAND);
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

