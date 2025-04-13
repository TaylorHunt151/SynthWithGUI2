#pragma once


#pragma once
enum class ScaleType {
    Linear,
    Logarithmic,
    Exponential
};

class KnobControl : public wxPanel {
public:

    // The knob defaults to m_min.
    KnobControl(wxWindow* parent, wxWindowID id,
        double minVal, double maxVal,
        ScaleType scaleType,
        const wxString& labelText)
        : wxPanel(parent, id, wxDefaultPosition, wxSize(60, 80), wxBORDER_SIMPLE),
        m_min(minVal), m_max(maxVal), m_scaleType(scaleType), m_label(labelText)
    {
        m_value = m_min;  // Default value equals the minimum.
        m_angle = AngleFromValue(m_value);
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &KnobControl::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &KnobControl::OnMouseDown, this);
        Bind(wxEVT_MOTION, &KnobControl::OnMouseMove, this);
        Bind(wxEVT_LEFT_UP, &KnobControl::OnMouseUp, this);
    }

    // Returns the current knob value.
    double GetValue() const { return m_value; }

    // Sets a new value (clamped to [m_min, m_max]) and refreshes the control.
    void SetValue(double newValue) {
        if (newValue < m_min)
            newValue = m_min;
        if (newValue > m_max)
            newValue = m_max;
        m_value = newValue;
        m_angle = AngleFromValue(m_value);
        Refresh();
    }

private:
    double m_min, m_max;      // The output range.
    double m_value;           // Current value.
    double m_angle;           // Internal angle corresponding to m_value.
    ScaleType m_scaleType;    // Chosen scaling type.
    wxString m_label;         // Label text drawn below the knob.

    int xPrev, yPrev;         // Previous mouse coordinates.
    bool m_dragging = false;  // True if dragging.

    // AngleFromValue maps a knob value to an angle between -135° and +135°.
    // It first calculates a normalized linear progress and then transforms it
    // based on the selected scale type.
    double AngleFromValue(double value) const {
        double linProg = (value - m_min) / (m_max - m_min);
        double transformedProg = 0.0;
        if (m_scaleType == ScaleType::Linear) {
            transformedProg = linProg;
        }
        else if (m_scaleType == ScaleType::Logarithmic) {
            // Concave mapping: square the linear progress.
            transformedProg = linProg * linProg;
        }
        else if (m_scaleType == ScaleType::Exponential) {
            // Convex mapping: square-root the linear progress.
            transformedProg = std::sqrt(linProg);
        }
        // Map transformed progress (0 to 1) to an angle between -135° and +135°.
        return transformedProg * 270.0 - 135.0;
    }

    // ValueFromAngle does the inverse mapping: from angle back to the knob value.
    double ValueFromAngle(double angle) const {
        double physProg = (angle + 135.0) / 270.0;  // normalized progress [0,1]
        double linProg = 0.0;
        if (m_scaleType == ScaleType::Linear) {
            linProg = physProg;
        }
        else if (m_scaleType == ScaleType::Logarithmic) {
            // Inverse of squaring is taking the square root.
            linProg = std::sqrt(physProg);
        }
        else if (m_scaleType == ScaleType::Exponential) {
            // Inverse of square-root is squaring.
            linProg = physProg * physProg;
        }
        return m_min + linProg * (m_max - m_min);
    }

    // OnPaint draws the knob, its indicator, and the label.
    void OnPaint(wxPaintEvent&) {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();

        int width = GetSize().GetWidth();
        int height = GetSize().GetHeight();

        // Reserve 20 pixels at the bottom for the label.
        int cx = width / 2;
        int cy = (height - 20) / 2;
        int radius = std::min(cx, cy) - 5;

        // Draw the knob background as a circle.
        dc.SetBrush(*wxLIGHT_GREY_BRUSH);
        dc.DrawCircle(cx, cy, radius);

        // Draw the indicator line.
        double rad = m_angle * M_PI / 180.0;
        int x2 = cx + static_cast<int>((radius - 5) * std::cos(rad));
        int y2 = cy - static_cast<int>((radius - 5) * std::sin(rad));
        dc.SetPen(wxPen(wxColour(0, 0, 0), 2));
        dc.DrawLine(cx, cy, x2, y2);

        // Draw the label underneath the knob.
        wxSize textSize = dc.GetTextExtent(m_label);
        int tx = (width - textSize.GetWidth()) / 2;
        int ty = height - textSize.GetHeight() - 5;
        dc.DrawText(m_label, tx, ty);
    }

    // Mouse event handlers.
    void OnMouseDown(wxMouseEvent& event) {
        wxPoint pos = event.GetPosition();
        xPrev = pos.x;
        yPrev = pos.y;
        m_dragging = true;
        CaptureMouse();
    }

    void OnMouseMove(wxMouseEvent& event) {
        if (m_dragging) {
            wxPoint pos = event.GetPosition();
            double delta = ((pos.y - yPrev) - (pos.x - xPrev));
            double newAngle = m_angle + delta;
            if (newAngle > 135) newAngle = 135;
            if (newAngle < -135) newAngle = -135;
            m_angle = newAngle;
            xPrev = pos.x;
            yPrev = pos.y;
            m_value = ValueFromAngle(m_angle);
            Refresh();
        }
    }

    void OnMouseUp(wxMouseEvent& event) {
        if (m_dragging) {
            m_dragging = false;
            ReleaseMouse();
        }
    }
};




extern KeyInputManager* keyInputManager;

// This custom control draws 16 keys (A to P) and uses the KeyInputManager for key states.
class PianoKeyboard : public wxPanel {
public:
    PianoKeyboard(wxWindow* parent, wxWindowID id = wxID_ANY)
        : wxPanel(parent, id, wxDefaultPosition, wxSize(800, 300))
    {
        // Set a background color so keys stand out.
        SetBackgroundColour(wxColour(220, 220, 220));
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &PianoKeyboard::OnPaint, this);
    }

private:
    // Row 1 definition (unchanged)
    const std::vector<char> whiteKeysRow1 = { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p' };
    const std::vector<char> blackKeysRow1 = { '2', '3', '5', '6', '7', '9', '0' };

    // Row 2 definition as specified:
    // The full sequence: z, S, x, D, c, v, G, b, H, n, J, m
    // White keys (full-size): z, x, c, v, b, n, m
    // Black keys (smaller): S, D, G, H, J
    const std::vector<char> whiteKeysRow2 = { 'z', 'x', 'c', 'v', 'b', 'n', 'm' };
    const std::vector<char> blackKeysRow2 = { 'S', 'D', 'G', 'H', 'J' };

    // Helper function: returns true if the given key (case-insensitive) is pressed.
    bool IsKeyPressed(char key) const {
        char upKey = toupper(key);
        return (std::find(keyInputManager->noteSet.begin(), keyInputManager->noteSet.end(), upKey) != keyInputManager->noteSet.end());
    }

    void OnPaint(wxPaintEvent& event) {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();
        wxSize sz = GetSize();
        int totalWidth = sz.GetWidth();
        int totalHeight = sz.GetHeight();

        // Split the panel vertically into two equal rows.
        int rowHeight = totalHeight / 2;

        // ========= Draw First Row =========
        int whiteCount1 = whiteKeysRow1.size(); // 10 white keys
        int whiteKeyWidth1 = totalWidth / whiteCount1;
        for (int i = 0; i < whiteCount1; i++) {
            int x = i * whiteKeyWidth1;
            bool pressed = IsKeyPressed(whiteKeysRow1[i]);
            dc.SetBrush(pressed ? *wxLIGHT_GREY_BRUSH : *wxWHITE_BRUSH);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawRectangle(x, 0, whiteKeyWidth1, rowHeight);

            wxString label; label.Printf("%c", whiteKeysRow1[i]);
            wxSize textSize = dc.GetTextExtent(label);
            int textX = x + (whiteKeyWidth1 - textSize.GetWidth()) / 2;
            int textY = rowHeight - textSize.GetHeight() - 5;
            dc.DrawText(label, textX, textY);
        }
        // Black keys for row 1:
        int blackKeyWidth1 = static_cast<int>(whiteKeyWidth1 * 0.6);
        int blackKeyHeight1 = static_cast<int>(rowHeight * 0.6);
        std::vector<int> blackX1;
        // Group 1:
        blackX1.push_back(whiteKeyWidth1 - blackKeyWidth1 / 2);
        blackX1.push_back(2 * whiteKeyWidth1 - blackKeyWidth1 / 2);
        // Group 2:
        blackX1.push_back(4 * whiteKeyWidth1 - blackKeyWidth1 / 2);
        blackX1.push_back(5 * whiteKeyWidth1 - blackKeyWidth1 / 2);
        blackX1.push_back(6 * whiteKeyWidth1 - blackKeyWidth1 / 2);
        // Group 3:
        blackX1.push_back(8 * whiteKeyWidth1 - blackKeyWidth1 / 2);
        blackX1.push_back(9 * whiteKeyWidth1 - blackKeyWidth1 / 2);

        for (size_t i = 0; i < blackKeysRow1.size() && i < blackX1.size(); i++) {
            int x = blackX1[i];
            bool pressed = IsKeyPressed(blackKeysRow1[i]);
            dc.SetBrush(pressed ? *wxLIGHT_GREY_BRUSH : *wxBLACK_BRUSH);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawRectangle(x, 0, blackKeyWidth1, blackKeyHeight1);

            wxString label; label.Printf("%c", blackKeysRow1[i]);
            wxSize textSize = dc.GetTextExtent(label);
            int textX = x + (blackKeyWidth1 - textSize.GetWidth()) / 2;
            int textY = (blackKeyHeight1 - textSize.GetHeight()) / 2;
            dc.SetTextForeground(*wxWHITE);
            dc.DrawText(label, textX, textY);
        }

        // ========= Draw Second Row =========
        int row2Y = rowHeight;  // Second row starts at y = rowHeight
        int whiteCount2 = whiteKeysRow2.size();  // 7 white keys
        int whiteKeyWidth2 = totalWidth / whiteCount2;
        for (int i = 0; i < whiteCount2; i++) {
            int x = i * whiteKeyWidth2;
            bool pressed = IsKeyPressed(whiteKeysRow2[i]);
            dc.SetBrush(pressed ? *wxLIGHT_GREY_BRUSH : *wxWHITE_BRUSH);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawRectangle(x, row2Y, whiteKeyWidth2, rowHeight);
            wxString label; label.Printf("%c", whiteKeysRow2[i]);

            // Explicitly set text color for white keys.
            dc.SetTextForeground(*wxBLACK);

            wxSize textSize = dc.GetTextExtent(label);
            int textX = x + (whiteKeyWidth2 - textSize.GetWidth()) / 2;
            int textY = row2Y + rowHeight - textSize.GetHeight() - 5;
            dc.DrawText(label, textX, textY);
        }
        // Black keys for row 2:
        int blackKeyWidth2 = static_cast<int>(whiteKeyWidth2 * 0.6);
        int blackKeyHeight2 = static_cast<int>(rowHeight * 0.6);
        // For the second row, white keys: [z, x, c, v, b, n, m]
        // Black keys to be drawn between them: S between z and x, D between x and c,
        // G between v and b, H between b and n, J between n and m.
        std::vector<int> blackX2;
        auto centerWhite = [&](int i) -> int {
            return i * whiteKeyWidth2 + whiteKeyWidth2 / 2;
            };
        // Calculate the X position for each black key:
        blackX2.push_back((centerWhite(0) + centerWhite(1)) / 2 - blackKeyWidth2 / 2); // S
        blackX2.push_back((centerWhite(1) + centerWhite(2)) / 2 - blackKeyWidth2 / 2); // D
        blackX2.push_back((centerWhite(3) + centerWhite(4)) / 2 - blackKeyWidth2 / 2); // G
        blackX2.push_back((centerWhite(4) + centerWhite(5)) / 2 - blackKeyWidth2 / 2); // H
        blackX2.push_back((centerWhite(5) + centerWhite(6)) / 2 - blackKeyWidth2 / 2); // J

        for (size_t i = 0; i < blackKeysRow2.size() && i < blackX2.size(); i++) {
            int x = blackX2[i];
            bool pressed = IsKeyPressed(blackKeysRow2[i]);
            dc.SetBrush(pressed ? *wxLIGHT_GREY_BRUSH : *wxBLACK_BRUSH);
            dc.SetPen(*wxBLACK_PEN);
            dc.DrawRectangle(x, row2Y, blackKeyWidth2, blackKeyHeight2);
            wxString label; label.Printf("%c", blackKeysRow2[i]);
            wxSize textSize = dc.GetTextExtent(label);
            int textX = x + (blackKeyWidth2 - textSize.GetWidth()) / 2;
            int textY = row2Y + (blackKeyHeight2 - textSize.GetHeight()) / 2;
            dc.SetTextForeground(*wxWHITE);
            dc.DrawText(label, textX, textY);
        }
    }
};