#pragma once


class KnobControl : public wxPanel {
public:
    KnobControl(wxWindow* parent, wxWindowID id = wxID_ANY, int minValue = 0, int maxValue = 100)
        : wxPanel(parent, id, wxDefaultPosition, wxSize(60, 60), wxBORDER_SIMPLE),
        minValue(minValue), maxValue(maxValue), value((minValue + maxValue) / 2), angle(0), varToChange(varToChange) {

        SetBackgroundStyle(wxBG_STYLE_PAINT); //Avoid flickering
        Bind(wxEVT_PAINT, &KnobControl::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &KnobControl::OnMouseDown, this);
        Bind(wxEVT_MOTION, &KnobControl::OnMouseMove, this);
        Bind(wxEVT_LEFT_UP, &KnobControl::OnMouseUp, this);
    }

    int GetValue() const { return value; }
    void SetValue(int newValue) {
        if (newValue < minValue) newValue = minValue;
        if (newValue > maxValue) newValue = maxValue;
        value = newValue;
        angle = (value - minValue) * 270.0 / (maxValue - minValue) - 135; //Map value to angle (-135� to 135�)
        Refresh();
    }

private:
    int minValue, maxValue, value, xPrevious, yPrevious;
    float& varToChange;
    double angle = 0;
    bool isDragging = false;


    void OnPaint(wxPaintEvent&) {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();
        dc.SetBrush(*wxLIGHT_GREY_BRUSH);
        dc.DrawCircle(30, 30, 20); //Draw the knob background

        //Draw indicator line based on angle
        double radians = angle * M_PI / 180.0;
        int x = 30 + 15 * cos(radians);
        int y = 30 - 15 * sin(radians);
        dc.SetPen(wxPen(*wxBLACK, 2));
        dc.DrawLine(30, 30, x, y);
    }

    void OnMouseDown(wxMouseEvent& event) {
        wxPoint pos = event.GetPosition();
        xPrevious = pos.x;
        yPrevious = pos.y;
        isDragging = true;
        CaptureMouse();
    }

    void OnMouseMove(wxMouseEvent& event) {
        if (isDragging) {
            wxPoint pos = event.GetPosition();

            // Calculate the delta based on mouse movement.
            int delta = ((pos.y - yPrevious) - (pos.x - xPrevious));

            // Compute a temporary new angle.
            double newAngle = angle + delta;

            // Clamp the newAngle to the allowed range [ -135, 135 ].
            if (newAngle > 135)
                newAngle = 135;
            else if (newAngle < -135)
                newAngle = -135;

            // Only update the angle if it hasn't hit the boundary or if the delta
            // would move it back within the range.
            angle = newAngle;

            // Update previous mouse positions.
            xPrevious = pos.x;
            yPrevious = pos.y;

            // Map the clamped angle to the knob value.
            value = minValue + (angle + 135) * (maxValue - minValue) / 270;

            Refresh();
        }
    }

    void OnMouseUp(wxMouseEvent&) {
        if (isDragging) {
            isDragging = false;
            ReleaseMouse();
        }
    }
};


extern KeyInputManager* keyInputManager;

// This custom control draws 16 keys (A to P) and uses the KeyInputManager for key states.
class PianoKeyboard : public wxPanel {
public:
    PianoKeyboard(wxWindow* parent, wxWindowID id = wxID_ANY)
        : wxPanel(parent, id, wxDefaultPosition, wxSize(600, 100))
    {
        // Use double-buffered painting to reduce flicker.
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &PianoKeyboard::OnPaint, this);
    }

private:
    // In OnPaint we directly query keyInputManager->noteSet which should have size at least 16.
    void OnPaint(wxPaintEvent&) {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();

        int numKeys = 16; // We display keys A to P.
        wxSize sz = GetSize();
        int keyWidth = sz.GetWidth() / numKeys;
        int keyHeight = sz.GetHeight();

        // Access the global keyInputManager noteSet.
        // (Ensure that keyInputManager is properly instantiated elsewhere.)
        std::vector<char> noteSet = keyInputManager->noteSet;

        for (int i = 0; i < numKeys; i++) {
            wxRect rect(i * keyWidth, 0, keyWidth, keyHeight);

            // Determine if this key is "pressed". If the noteSet at this index is not blank (' '), treat it as pressed.
            bool pressed = (i < static_cast<int>(noteSet.size()) && noteSet[i] != ' ');

            // Set brush color based on pressed state.
            dc.SetBrush(pressed ? *wxBLUE_BRUSH : *wxWHITE_BRUSH);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawRectangle(rect);

            // Label: If pressed, use the actual key from noteSet; otherwise default to the letter (A + i)
            wxString label;
            if (pressed)
                label.Printf("%c", noteSet[i]);
            else
                label.Printf("%c", 'A' + i);

            wxSize textSize = dc.GetTextExtent(label);
            dc.DrawText(label,
                rect.x + (keyWidth - textSize.GetWidth()) / 2,
                rect.y + (keyHeight - textSize.GetHeight()) / 2);
        }
    }
};
