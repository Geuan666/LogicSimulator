#include "../../include/components/decoder_encoder_components.h"
#include <wx/dc.h>

// 3-to-8 Line Decoder implementation
Decoder3to8::Decoder3to8(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(80, 120), ComponentType::DECODER_3TO8) {

    // 3 input pins (A2, A1, A0), Enable pin, 8 output pins
    pins.emplace_back(wxPoint(pos.x, pos.y + 20), true);      // A0
    pins.emplace_back(wxPoint(pos.x, pos.y + 40), true);      // A1
    pins.emplace_back(wxPoint(pos.x, pos.y + 60), true);      // A2
    pins.emplace_back(wxPoint(pos.x, pos.y + 100), true);     // Enable

    // 8 output pins
    for (int i = 0; i < 8; ++i) {
        pins.emplace_back(wxPoint(pos.x + 80, pos.y + 10 + i * 12), false);
    }
}

void Decoder3to8::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxWHITE));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw label
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.DrawText("3:8", pos.x + 25, pos.y + 45);
    dc.DrawText("DEC", pos.x + 20, pos.y + 60);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("A0", pos.x - 20, pos.y + 15);
    dc.DrawText("A1", pos.x - 20, pos.y + 35);
    dc.DrawText("A2", pos.x - 20, pos.y + 55);
    dc.DrawText("EN", pos.x - 20, pos.y + 95);

    // Output labels
    for (int i = 0; i < 8; ++i) {
        dc.DrawText(wxString::Format("Y%d", i), pos.x + 85, pos.y + 5 + i * 12);
    }

    // Draw pins
    dc.SetBrush(wxBrush(*wxBLACK));
    for (const auto& pin : pins) {
        wxColour pinColor = pin.isConnected ? *wxRED : *wxBLACK;
        dc.SetPen(wxPen(pinColor, 2));
        dc.DrawCircle(pin.position, 3);
    }

    // Highlight if selected
    if (selected) {
        dc.SetPen(wxPen(*wxRED, 3, wxPENSTYLE_DOT));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(pos.x - 5, pos.y - 5, size.x + 10, size.y + 10);
    }
}

void Decoder3to8::ComputeOutputs() {
    if (pins.size() >= 12) {
        LogicValue enable = pins[3].value;

        if (enable != LogicValue::HIGH) {
            // Decoder disabled - all outputs LOW
            for (int i = 4; i < 12; ++i) {
                pins[i].value = LogicValue::LOW;
            }
            return;
        }

        // Get input values
        int a0 = (pins[0].value == LogicValue::HIGH) ? 1 : 0;
        int a1 = (pins[1].value == LogicValue::HIGH) ? 1 : 0;
        int a2 = (pins[2].value == LogicValue::HIGH) ? 1 : 0;

        int selectedOutput = a0 + (a1 << 1) + (a2 << 2);

        // Set all outputs to LOW first
        for (int i = 4; i < 12; ++i) {
            pins[i].value = LogicValue::LOW;
        }

        // Set selected output to HIGH
        if (selectedOutput >= 0 && selectedOutput < 8) {
            pins[4 + selectedOutput].value = LogicValue::HIGH;
        }
    }
}

LogicValue Decoder3to8::GetOutput(int index) const {
    if (index >= 0 && index < 8 && pins.size() >= 12) {
        return pins[4 + index].value;
    }
    return LogicValue::UNDEFINED;
}

// BCD to 7-Segment Decoder implementation
BCDTo7SegmentDecoder::BCDTo7SegmentDecoder(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(80, 100), ComponentType::DECODER_2TO4) {

    // 4 BCD input pins
    pins.emplace_back(wxPoint(pos.x, pos.y + 20), true);      // D0
    pins.emplace_back(wxPoint(pos.x, pos.y + 35), true);      // D1
    pins.emplace_back(wxPoint(pos.x, pos.y + 50), true);      // D2
    pins.emplace_back(wxPoint(pos.x, pos.y + 65), true);      // D3

    // 7 segment output pins (a, b, c, d, e, f, g)
    for (int i = 0; i < 7; ++i) {
        pins.emplace_back(wxPoint(pos.x + 80, pos.y + 15 + i * 10), false);
    }

    // Initialize outputs
    for (int i = 0; i < 7; ++i) {
        outputs[i] = false;
    }
}

void BCDTo7SegmentDecoder::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxWHITE));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw 7-segment display representation
    dc.SetPen(wxPen(*wxBLACK, 1));
    // Draw a small 7-segment display inside
    int segX = pos.x + 25;
    int segY = pos.y + 25;
    int segW = 20;
    int segH = 30;

    // Segment positions (a, b, c, d, e, f, g)
    // a - top
    dc.SetPen(wxPen(outputs[0] ? *wxRED : wxColour(200, 200, 200), 2));
    dc.DrawLine(segX + 2, segY, segX + segW - 2, segY);

    // b - top right
    dc.SetPen(wxPen(outputs[1] ? *wxRED : wxColour(200, 200, 200), 2));
    dc.DrawLine(segX + segW, segY + 2, segX + segW, segY + segH/2 - 2);

    // c - bottom right
    dc.SetPen(wxPen(outputs[2] ? *wxRED : wxColour(200, 200, 200), 2));
    dc.DrawLine(segX + segW, segY + segH/2 + 2, segX + segW, segY + segH - 2);

    // d - bottom
    dc.SetPen(wxPen(outputs[3] ? *wxRED : wxColour(200, 200, 200), 2));
    dc.DrawLine(segX + 2, segY + segH, segX + segW - 2, segY + segH);

    // e - bottom left
    dc.SetPen(wxPen(outputs[4] ? *wxRED : wxColour(200, 200, 200), 2));
    dc.DrawLine(segX, segY + segH/2 + 2, segX, segY + segH - 2);

    // f - top left
    dc.SetPen(wxPen(outputs[5] ? *wxRED : wxColour(200, 200, 200), 2));
    dc.DrawLine(segX, segY + 2, segX, segY + segH/2 - 2);

    // g - middle
    dc.SetPen(wxPen(outputs[6] ? *wxRED : wxColour(200, 200, 200), 2));
    dc.DrawLine(segX + 2, segY + segH/2, segX + segW - 2, segY + segH/2);

    // Draw labels
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.DrawText("BCD", pos.x + 5, pos.y + 5);
    dc.DrawText("7SEG", pos.x + 5, pos.y + 80);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("D0", pos.x - 20, pos.y + 15);
    dc.DrawText("D1", pos.x - 20, pos.y + 30);
    dc.DrawText("D2", pos.x - 20, pos.y + 45);
    dc.DrawText("D3", pos.x - 20, pos.y + 60);

    // Output labels (a-g)
    char segNames[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    for (int i = 0; i < 7; ++i) {
        dc.DrawText(wxString::Format("%c", segNames[i]), pos.x + 85, pos.y + 10 + i * 10);
    }

    // Draw pins
    dc.SetBrush(wxBrush(*wxBLACK));
    for (const auto& pin : pins) {
        wxColour pinColor = pin.isConnected ? *wxRED : *wxBLACK;
        dc.SetPen(wxPen(pinColor, 2));
        dc.DrawCircle(pin.position, 3);
    }

    // Highlight if selected
    if (selected) {
        dc.SetPen(wxPen(*wxRED, 3, wxPENSTYLE_DOT));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(pos.x - 5, pos.y - 5, size.x + 10, size.y + 10);
    }
}

void BCDTo7SegmentDecoder::ComputeOutputs() {
    if (pins.size() >= 11) {
        // Get BCD input
        int bcd = 0;
        for (int i = 0; i < 4; ++i) {
            if (pins[i].value == LogicValue::HIGH) {
                bcd |= (1 << i);
            }
        }

        // BCD to 7-segment truth table
        // segments: a, b, c, d, e, f, g
        bool segmentPatterns[16][7] = {
            // 0: a b c d e f g
            {1, 1, 1, 1, 1, 1, 0}, // 0
            {0, 1, 1, 0, 0, 0, 0}, // 1
            {1, 1, 0, 1, 1, 0, 1}, // 2
            {1, 1, 1, 1, 0, 0, 1}, // 3
            {0, 1, 1, 0, 0, 1, 1}, // 4
            {1, 0, 1, 1, 0, 1, 1}, // 5
            {1, 0, 1, 1, 1, 1, 1}, // 6
            {1, 1, 1, 0, 0, 0, 0}, // 7
            {1, 1, 1, 1, 1, 1, 1}, // 8
            {1, 1, 1, 1, 0, 1, 1}, // 9
            {0, 0, 0, 0, 0, 0, 0}, // A-F (blank for invalid BCD)
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0}
        };

        // Set outputs
        for (int i = 0; i < 7; ++i) {
            outputs[i] = segmentPatterns[bcd][i];
            pins[4 + i].value = outputs[i] ? LogicValue::HIGH : LogicValue::LOW;
        }
    }
}

bool BCDTo7SegmentDecoder::GetSegment(int segment) const {
    if (segment >= 0 && segment < 7) {
        return outputs[segment];
    }
    return false;
}

// 8-to-3 Priority Encoder implementation
PriorityEncoder8to3::PriorityEncoder8to3(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(80, 120), ComponentType::ENCODER_4TO2) {

    // 8 input pins
    for (int i = 0; i < 8; ++i) {
        pins.emplace_back(wxPoint(pos.x, pos.y + 10 + i * 12), true);
    }

    // 3 output pins (A2, A1, A0) + Valid output
    pins.emplace_back(wxPoint(pos.x + 80, pos.y + 30), false); // A0
    pins.emplace_back(wxPoint(pos.x + 80, pos.y + 50), false); // A1
    pins.emplace_back(wxPoint(pos.x + 80, pos.y + 70), false); // A2
    pins.emplace_back(wxPoint(pos.x + 80, pos.y + 90), false); // Valid
}

void PriorityEncoder8to3::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxWHITE));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw label
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.DrawText("8:3", pos.x + 25, pos.y + 45);
    dc.DrawText("ENC", pos.x + 20, pos.y + 60);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    for (int i = 0; i < 8; ++i) {
        dc.DrawText(wxString::Format("I%d", i), pos.x - 20, pos.y + 5 + i * 12);
    }

    // Output labels
    dc.DrawText("A0", pos.x + 85, pos.y + 25);
    dc.DrawText("A1", pos.x + 85, pos.y + 45);
    dc.DrawText("A2", pos.x + 85, pos.y + 65);
    dc.DrawText("V", pos.x + 85, pos.y + 85);

    // Draw pins
    dc.SetBrush(wxBrush(*wxBLACK));
    for (const auto& pin : pins) {
        wxColour pinColor = pin.isConnected ? *wxRED : *wxBLACK;
        dc.SetPen(wxPen(pinColor, 2));
        dc.DrawCircle(pin.position, 3);
    }

    // Highlight if selected
    if (selected) {
        dc.SetPen(wxPen(*wxRED, 3, wxPENSTYLE_DOT));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(pos.x - 5, pos.y - 5, size.x + 10, size.y + 10);
    }
}

void PriorityEncoder8to3::ComputeOutputs() {
    if (pins.size() >= 12) {
        int highestPriority = -1;

        // Find highest priority input (highest index)
        for (int i = 7; i >= 0; --i) {
            if (pins[i].value == LogicValue::HIGH) {
                highestPriority = i;
                break;
            }
        }

        if (highestPriority >= 0) {
            // Valid input found
            pins[8].value = (highestPriority & 1) ? LogicValue::HIGH : LogicValue::LOW;   // A0
            pins[9].value = (highestPriority & 2) ? LogicValue::HIGH : LogicValue::LOW;   // A1
            pins[10].value = (highestPriority & 4) ? LogicValue::HIGH : LogicValue::LOW;  // A2
            pins[11].value = LogicValue::HIGH; // Valid
        } else {
            // No valid input
            pins[8].value = LogicValue::LOW;   // A0
            pins[9].value = LogicValue::LOW;   // A1
            pins[10].value = LogicValue::LOW;  // A2
            pins[11].value = LogicValue::LOW;  // Valid
        }
    }
}

LogicValue PriorityEncoder8to3::GetOutput(int index) const {
    if (index >= 0 && index < 3 && pins.size() >= 12) {
        return pins[8 + index].value;
    }
    return LogicValue::UNDEFINED;
}

bool PriorityEncoder8to3::IsValidInput() const {
    if (pins.size() >= 12) {
        return pins[11].value == LogicValue::HIGH;
    }
    return false;
}