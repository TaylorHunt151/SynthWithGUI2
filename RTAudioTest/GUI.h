//#pragma once
//#include <wx/wx.h>
//
///*******************************************************************
//* NOTE: I moved the GUI code over to its own file so main.cpp is
//* not so cluttered.
//********************************************************************/
//
//class KnobControl : public wxPanel {
//public:
//    KnobControl(wxWindow* parent, wxWindowID id = wxID_ANY, int minValue = 0, int maxValue = 100)
//        : wxPanel(parent, id, wxDefaultPosition, wxSize(60, 60), wxBORDER_SIMPLE),
//        minValue(minValue), maxValue(maxValue), value((minValue + maxValue) / 2), angle(0), varToChange(varToChange) {
//
//        SetBackgroundStyle(wxBG_STYLE_PAINT); //Avoid flickering
//        Bind(wxEVT_PAINT, &KnobControl::OnPaint, this);
//        Bind(wxEVT_LEFT_DOWN, &KnobControl::OnMouseDown, this);
//        Bind(wxEVT_MOTION, &KnobControl::OnMouseMove, this);
//        Bind(wxEVT_LEFT_UP, &KnobControl::OnMouseUp, this);
//    }
//
//    int GetValue() const { return value; }
//    void SetValue(int newValue) {
//        if (newValue < minValue) newValue = minValue;
//        if (newValue > maxValue) newValue = maxValue;
//        value = newValue;
//        angle = (value - minValue) * 270.0 / (maxValue - minValue) - 135; //Map value to angle (-135° to 135°)
//        Refresh();
//    }
//
//private:
//    int minValue, maxValue, value, xPrevious, yPrevious;
//    float& varToChange;
//    double angle = 0;
//    bool isDragging = false;
//
//
//    void OnPaint(wxPaintEvent&) {
//        wxAutoBufferedPaintDC dc(this);
//        dc.Clear();
//        dc.SetBrush(*wxLIGHT_GREY_BRUSH);
//        dc.DrawCircle(30, 30, 20); //Draw the knob background
//
//        //Draw indicator line based on angle
//        double radians = angle * M_PI / 180.0;
//        int x = 30 + 15 * cos(radians);
//        int y = 30 - 15 * sin(radians);
//        dc.SetPen(wxPen(*wxBLACK, 2));
//        dc.DrawLine(30, 30, x, y);
//    }
//
//    void OnMouseDown(wxMouseEvent& event) {
//        wxPoint pos = event.GetPosition();
//        xPrevious = pos.x;
//        yPrevious = pos.y;
//        isDragging = true;
//        CaptureMouse();
//    }
//
//    void OnMouseMove(wxMouseEvent& event) {
//        if (isDragging) {
//            wxPoint pos = event.GetPosition();
//
//            //double newAngle = atan2(30 - pos.y, pos.x - 30) * 180 / M_PI;
//            //newAngle = wxClip(newAngle, -135, 135); //Limit rotation
//            //angle = newAngle;
//            angle += ((pos.y - yPrevious) - (pos.x - xPrevious)); //New equation for the knob angle. When the mouse moves up or to the right, the knob turns clockwise. When it moves down or to the left, the knob turns counter-clockwise.
//            xPrevious = pos.x;
//            yPrevious = pos.y;
//            //Map angle to value
//            value = minValue + (angle + 135) * (maxValue - minValue) / 270;
//            if (value < minValue) {
//                value = minValue;
//            }
//            else if (value > maxValue) {
//                value = maxValue;
//            }
//            //varToChange = value;
//            Refresh();
//        }
//    }
//
//    void OnMouseUp(wxMouseEvent&) {
//        if (isDragging) {
//            isDragging = false;
//            ReleaseMouse();
//        }
//    }
//};
//
//
//void guiSetup(wxPanel* panel, wxFrame* window) {
//    // Existing volume knob
//    KnobControl* volumeKnob = new KnobControl(panel, wxID_ANY, 0, 100);
//    volumeKnob->SetValue(50);
//
//    // Cutoff knob
//    KnobControl* cutoffKnob = new KnobControl(panel, wxID_ANY, 80, 18000);
//    cutoffKnob->SetValue(filtCutoff);
//
//    // Debug box (multiline, readonly)
//    wxTextCtrl* debugBox = new wxTextCtrl(panel, wxID_ANY, "",
//        wxDefaultPosition, wxSize(200, 100),
//        wxTE_MULTILINE | wxTE_READONLY | wxBORDER_SIMPLE);
//
//    // Main vertical sizer to organize everything
//    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
//    wxBoxSizer* knobSizer = new wxBoxSizer(wxHORIZONTAL);
//    knobSizer->Add(volumeKnob, 0, wxALL, 15);
//    knobSizer->Add(cutoffKnob, 0, wxALL, 15);
//
//    mainSizer->Add(knobSizer, 0, wxALIGN_CENTER);
//
//    // Align debug box to bottom-right corner
//    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
//    bottomSizer->AddStretchSpacer(1);
//    bottomSizer->Add(debugBox, 0, wxALL | wxALIGN_RIGHT | wxALIGN_BOTTOM, 10);
//
//    mainSizer->AddStretchSpacer(1);
//    mainSizer->Add(bottomSizer, 0, wxEXPAND);
//
//    panel->SetSizer(mainSizer);
//
//    window->Bind(wxEVT_TIMER, [=](wxTimerEvent&) {
//        //cutoffSet.store(static_cast<double>(cutoffKnob->GetValue()));
//        filtCutoff = static_cast<double>(cutoffKnob->GetValue());
//
//        // Output current cutoffSet value clearly for debugging
//        debugBox->SetValue(wxString::Format("Cutoff: %.2f\nVolume: %d",
//            filtCutoff, volumeKnob->GetValue()));
//        });
//}