#include "../../include/components/display_components.h"
#include <wx/dc.h>
#include <cstring>

// Seven-Segment Display implementation
SevenSegmentDisplay::SevenSegmentDisplay(const wxPoint& pos, bool commonCathode)
    : CircuitComponent(pos, wxSize(60, 80), ComponentType::SEVEN_SEGMENT_DISPLAY),
      commonCathode(commonCathode), onColor(*wxRED), offColor(wxColour(100, 100, 100)) {

    // 7 segment input pins (a, b, c, d, e, f, g)
    for (int i = 0; i < 7; ++i) {
        pins.emplace_back(wxPoint(pos.x, pos.y + 10 + i * 10), true);
        segments[i] = false;
    }
}

void SevenSegmentDisplay::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle (background)
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxBLACK));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Seven-segment layout
    int segmentWidth = 30;
    int segmentHeight = 4;
    int centerX = pos.x + 30;
    int centerY = pos.y + 40;

    // Draw segments
    dc.SetPen(wxPen(*wxBLACK, 1));

    // Segment a (top)
    dc.SetBrush(wxBrush(segments[0] ? onColor : offColor));
    wxPoint topSegment[] = {
        wxPoint(centerX - segmentWidth/2, centerY - 20),
        wxPoint(centerX + segmentWidth/2, centerY - 20),
        wxPoint(centerX + segmentWidth/2 - 3, centerY - 20 + segmentHeight),
        wxPoint(centerX - segmentWidth/2 + 3, centerY - 20 + segmentHeight)
    };
    dc.DrawPolygon(4, topSegment);

    // Segment b (top right)
    dc.SetBrush(wxBrush(segments[1] ? onColor : offColor));
    wxPoint topRightSegment[] = {
        wxPoint(centerX + segmentWidth/2, centerY - 20),
        wxPoint(centerX + segmentWidth/2, centerY),
        wxPoint(centerX + segmentWidth/2 - segmentHeight, centerY - 3),
        wxPoint(centerX + segmentWidth/2 - segmentHeight, centerY - 20 + 3)
    };
    dc.DrawPolygon(4, topRightSegment);

    // Segment c (bottom right)
    dc.SetBrush(wxBrush(segments[2] ? onColor : offColor));
    wxPoint bottomRightSegment[] = {
        wxPoint(centerX + segmentWidth/2, centerY),
        wxPoint(centerX + segmentWidth/2, centerY + 20),
        wxPoint(centerX + segmentWidth/2 - segmentHeight, centerY + 20 - 3),
        wxPoint(centerX + segmentWidth/2 - segmentHeight, centerY + 3)
    };
    dc.DrawPolygon(4, bottomRightSegment);

    // Segment d (bottom)
    dc.SetBrush(wxBrush(segments[3] ? onColor : offColor));
    wxPoint bottomSegment[] = {
        wxPoint(centerX - segmentWidth/2 + 3, centerY + 20 - segmentHeight),
        wxPoint(centerX + segmentWidth/2 - 3, centerY + 20 - segmentHeight),
        wxPoint(centerX + segmentWidth/2, centerY + 20),
        wxPoint(centerX - segmentWidth/2, centerY + 20)
    };
    dc.DrawPolygon(4, bottomSegment);

    // Segment e (bottom left)
    dc.SetBrush(wxBrush(segments[4] ? onColor : offColor));
    wxPoint bottomLeftSegment[] = {
        wxPoint(centerX - segmentWidth/2 + segmentHeight, centerY + 3),
        wxPoint(centerX - segmentWidth/2 + segmentHeight, centerY + 20 - 3),
        wxPoint(centerX - segmentWidth/2, centerY + 20),
        wxPoint(centerX - segmentWidth/2, centerY)
    };
    dc.DrawPolygon(4, bottomLeftSegment);

    // Segment f (top left)
    dc.SetBrush(wxBrush(segments[5] ? onColor : offColor));
    wxPoint topLeftSegment[] = {
        wxPoint(centerX - segmentWidth/2 + segmentHeight, centerY - 20 + 3),
        wxPoint(centerX - segmentWidth/2 + segmentHeight, centerY - 3),
        wxPoint(centerX - segmentWidth/2, centerY),
        wxPoint(centerX - segmentWidth/2, centerY - 20)
    };
    dc.DrawPolygon(4, topLeftSegment);

    // Segment g (middle)
    dc.SetBrush(wxBrush(segments[6] ? onColor : offColor));
    wxPoint middleSegment[] = {
        wxPoint(centerX - segmentWidth/2 + 3, centerY - segmentHeight/2),
        wxPoint(centerX + segmentWidth/2 - 3, centerY - segmentHeight/2),
        wxPoint(centerX + segmentWidth/2 - 6, centerY + segmentHeight/2),
        wxPoint(centerX - segmentWidth/2 + 6, centerY + segmentHeight/2)
    };
    dc.DrawPolygon(4, middleSegment);

    // Draw pin labels
    dc.SetTextForeground(*wxWHITE);
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    char segNames[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    for (int i = 0; i < 7; ++i) {
        dc.DrawText(wxString::Format("%c", segNames[i]), pos.x - 15, pos.y + 5 + i * 10);
    }

    // Draw pins
    dc.SetBrush(wxBrush(*wxBLACK));
    for (const auto& pin : pins) {
        wxColour pinColor = pin.isConnected ? *wxRED : *wxWHITE;
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

void SevenSegmentDisplay::UpdateDisplay() {
    if (pins.size() >= 7) {
        for (int i = 0; i < 7; ++i) {
            bool pinActive = (pins[i].value == LogicValue::HIGH);
            segments[i] = commonCathode ? pinActive : !pinActive;
        }
    }
}

void SevenSegmentDisplay::SetSegment(int segment, bool state) {
    if (segment >= 0 && segment < 7) {
        segments[segment] = state;
    }
}

bool SevenSegmentDisplay::GetSegment(int segment) const {
    if (segment >= 0 && segment < 7) {
        return segments[segment];
    }
    return false;
}

void SevenSegmentDisplay::SetColors(const wxColour& on, const wxColour& off) {
    onColor = on;
    offColor = off;
}

// LED Matrix 8x8 implementation
LEDMatrix8x8::LEDMatrix8x8(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(120, 120), ComponentType::LED_MATRIX_8X8),
      onColor(*wxRED), offColor(wxColour(50, 50, 50)), currentRow(0), currentCol(0) {

    // 8 row pins + 8 column pins
    for (int i = 0; i < 8; ++i) {
        pins.emplace_back(wxPoint(pos.x, pos.y + 10 + i * 12), true);        // Rows
        pins.emplace_back(wxPoint(pos.x + 10 + i * 12, pos.y + 120), true);  // Columns
    }

    // Initialize matrix
    Clear();
}

void LEDMatrix8x8::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxBLACK));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw LED matrix
    int ledSize = 10;
    int spacing = 12;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            wxColour ledColor = leds[row][col] ? onColor : offColor;
            dc.SetBrush(wxBrush(ledColor));
            dc.SetPen(wxPen(ledColor, 1));

            int x = pos.x + 10 + col * spacing;
            int y = pos.y + 10 + row * spacing;
            dc.DrawCircle(x + ledSize/2, y + ledSize/2, ledSize/2);
        }
    }

    // Draw pin labels
    dc.SetTextForeground(*wxWHITE);
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    for (int i = 0; i < 8; ++i) {
        dc.DrawText(wxString::Format("R%d", i), pos.x - 20, pos.y + 5 + i * 12);
        dc.DrawText(wxString::Format("C%d", i), pos.x + 5 + i * 12, pos.y + 125);
    }

    // Draw pins
    dc.SetBrush(wxBrush(*wxBLACK));
    for (const auto& pin : pins) {
        wxColour pinColor = pin.isConnected ? *wxRED : *wxWHITE;
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

void LEDMatrix8x8::UpdateMatrix() {
    if (pins.size() >= 16) {
        // Simple matrix scanning - check row and column inputs
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                bool rowActive = (pins[row].value == LogicValue::HIGH);
                bool colActive = (pins[8 + col].value == LogicValue::HIGH);
                leds[row][col] = rowActive && colActive;
            }
        }
    }
}

void LEDMatrix8x8::SetLED(int row, int col, bool state) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        leds[row][col] = state;
    }
}

bool LEDMatrix8x8::GetLED(int row, int col) const {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return leds[row][col];
    }
    return false;
}

void LEDMatrix8x8::SetColors(const wxColour& on, const wxColour& off) {
    onColor = on;
    offColor = off;
}

void LEDMatrix8x8::Clear() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            leds[row][col] = false;
        }
    }
}

void LEDMatrix8x8::SetPattern(const bool pattern[8][8]) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            leds[row][col] = pattern[row][col];
        }
    }
}

// Hex Display implementation
HexDisplay::HexDisplay(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(40, 40), ComponentType::SEVEN_SEGMENT_DISPLAY),
      value(0), backgroundColor(*wxBLACK), textColor(*wxGREEN) {

    // 4 input pins for 4-bit value
    for (int i = 0; i < 4; ++i) {
        pins.emplace_back(wxPoint(pos.x, pos.y + 5 + i * 8), true);
    }
}

void HexDisplay::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(backgroundColor));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw hex character
    dc.SetTextForeground(textColor);
    dc.SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    wxString hexChar = wxString::Format("%c", GetHexChar());
    wxSize textSize = dc.GetTextExtent(hexChar);
    dc.DrawText(hexChar,
                pos.x + (size.x - textSize.x) / 2,
                pos.y + (size.y - textSize.y) / 2);

    // Draw pin labels
    dc.SetTextForeground(*wxWHITE);
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    for (int i = 0; i < 4; ++i) {
        dc.DrawText(wxString::Format("%d", i), pos.x - 15, pos.y + i * 8);
    }

    // Draw pins
    dc.SetBrush(wxBrush(*wxBLACK));
    for (const auto& pin : pins) {
        wxColour pinColor = pin.isConnected ? *wxRED : *wxWHITE;
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

void HexDisplay::UpdateValue() {
    if (pins.size() >= 4) {
        value = 0;
        for (int i = 0; i < 4; ++i) {
            if (pins[i].value == LogicValue::HIGH) {
                value |= (1 << i);
            }
        }
    }
}

char HexDisplay::GetHexChar() const {
    if (value >= 0 && value <= 9) {
        return '0' + value;
    } else if (value >= 10 && value <= 15) {
        return 'A' + (value - 10);
    }
    return '?';
}

// Binary Display implementation
BinaryDisplay8Bit::BinaryDisplay8Bit(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(120, 30), ComponentType::SEVEN_SEGMENT_DISPLAY),
      onColor(*wxRED), offColor(wxColour(100, 100, 100)) {

    // 8 input pins for 8-bit value
    for (int i = 0; i < 8; ++i) {
        pins.emplace_back(wxPoint(pos.x, pos.y + 5 + i * 3), true);
        bits[i] = false;
    }
}

void BinaryDisplay8Bit::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxBLACK));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw bit indicators
    for (int i = 0; i < 8; ++i) {
        wxColour bitColor = bits[7-i] ? onColor : offColor; // MSB first
        dc.SetBrush(wxBrush(bitColor));
        dc.SetPen(wxPen(bitColor, 1));

        int x = pos.x + 10 + i * 12;
        int y = pos.y + 10;
        dc.DrawCircle(x, y, 8);

        // Draw bit number
        dc.SetTextForeground(*wxWHITE);
        dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        dc.DrawText(wxString::Format("%d", 7-i), x-3, y-4);
    }

    // Draw pins
    dc.SetBrush(wxBrush(*wxBLACK));
    for (const auto& pin : pins) {
        wxColour pinColor = pin.isConnected ? *wxRED : *wxWHITE;
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

void BinaryDisplay8Bit::UpdateBits() {
    if (pins.size() >= 8) {
        for (int i = 0; i < 8; ++i) {
            bits[i] = (pins[i].value == LogicValue::HIGH);
        }
    }
}

void BinaryDisplay8Bit::SetBit(int bit, bool state) {
    if (bit >= 0 && bit < 8) {
        bits[bit] = state;
    }
}

bool BinaryDisplay8Bit::GetBit(int bit) const {
    if (bit >= 0 && bit < 8) {
        return bits[bit];
    }
    return false;
}

void BinaryDisplay8Bit::SetValue(int value) {
    for (int i = 0; i < 8; ++i) {
        bits[i] = (value & (1 << i)) != 0;
    }
}

int BinaryDisplay8Bit::GetValue() const {
    int value = 0;
    for (int i = 0; i < 8; ++i) {
        if (bits[i]) {
            value |= (1 << i);
        }
    }
    return value;
}