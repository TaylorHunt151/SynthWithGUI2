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

	//panel->SetSizer(voiceSizer);

}

