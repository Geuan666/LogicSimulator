#include "../../include/components/arithmetic_components.h"

ArithmeticComponent::ArithmeticComponent(const wxPoint& pos, const wxSize& sz, ComponentType t, const std::string& lbl)
    : CircuitComponent(pos, sz, t), label(lbl) {
}

// Half Adder Implementation
HalfAdder::HalfAdder(const wxPoint& pos)
    : ArithmeticComponent(pos, wxSize(60, 50), ComponentType::HALF_ADDER, "HA") {
    // Create input pins: A and B
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));      // A
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 35), true));      // B

    // Create output pins: Sum and Carry
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 15), false));  // Sum
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 35), false));  // Carry
}

void HalfAdder::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 255, 240)));

    // Draw main body
    dc.DrawRectangle(position.x, position.y, size.x, size.y);

    // Draw label
    dc.DrawText(label, position.x + 20, position.y + 20);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("A", position.x - 10, position.y + 8);
    dc.DrawText("B", position.x - 10, position.y + 33);
    dc.DrawText("S", position.x + 65, position.y + 13);
    dc.DrawText("C", position.x + 65, position.y + 33);
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

void HalfAdder::ComputeOutputs() {
    LogicValue a = pins[0].value;
    LogicValue b = pins[1].value;

    // Sum = A XOR B
    if ((a == LogicValue::HIGH && b == LogicValue::LOW) ||
        (a == LogicValue::LOW && b == LogicValue::HIGH)) {
        pins[2].value = LogicValue::HIGH;
    } else {
        pins[2].value = LogicValue::LOW;
    }

    // Carry = A AND B
    if (a == LogicValue::HIGH && b == LogicValue::HIGH) {
        pins[3].value = LogicValue::HIGH;
    } else {
        pins[3].value = LogicValue::LOW;
    }
}

// Full Adder Implementation
FullAdder::FullAdder(const wxPoint& pos)
    : ArithmeticComponent(pos, wxSize(60, 60), ComponentType::FULL_ADDER, "FA") {
    // Create input pins: A, B, Cin
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));      // A
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 30), true));      // B
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 50), true));      // Cin

    // Create output pins: Sum and Cout
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 20), false));  // Sum
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 40), false));  // Cout
}

void FullAdder::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 255, 240)));

    // Draw main body
    dc.DrawRectangle(position.x, position.y, size.x, size.y);

    // Draw label
    dc.DrawText(label, position.x + 22, position.y + 25);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("A", position.x - 10, position.y + 8);
    dc.DrawText("B", position.x - 10, position.y + 28);
    dc.DrawText("Ci", position.x - 15, position.y + 48);
    dc.DrawText("S", position.x + 65, position.y + 18);
    dc.DrawText("Co", position.x + 65, position.y + 38);
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

void FullAdder::ComputeOutputs() {
    LogicValue a = pins[0].value;
    LogicValue b = pins[1].value;
    LogicValue cin = pins[2].value;

    // Convert to binary values
    int aVal = (a == LogicValue::HIGH) ? 1 : 0;
    int bVal = (b == LogicValue::HIGH) ? 1 : 0;
    int cinVal = (cin == LogicValue::HIGH) ? 1 : 0;

    int sum = aVal + bVal + cinVal;

    // Sum output
    pins[3].value = (sum % 2 == 1) ? LogicValue::HIGH : LogicValue::LOW;

    // Carry output
    pins[4].value = (sum >= 2) ? LogicValue::HIGH : LogicValue::LOW;
}

// 4-bit Adder Implementation
Adder4Bit::Adder4Bit(const wxPoint& pos)
    : ArithmeticComponent(pos, wxSize(80, 120), ComponentType::ADDER_4BIT, "4-bit ADD") {
    // Create input pins: A3-A0, B3-B0, Cin
    for (int i = 0; i < 4; i++) {
        pins.push_back(Pin(wxPoint(pos.x, pos.y + 10 + i * 20), true));  // A3-A0
    }
    for (int i = 0; i < 4; i++) {
        pins.push_back(Pin(wxPoint(pos.x, pos.y + 90 + i * 20), true));  // B3-B0
    }
    pins.push_back(Pin(wxPoint(pos.x + 40, pos.y + 170), true));         // Cin

    // Create output pins: S3-S0, Cout
    for (int i = 0; i < 4; i++) {
        pins.push_back(Pin(wxPoint(pos.x + 80, pos.y + 20 + i * 20), false)); // S3-S0
    }
    pins.push_back(Pin(wxPoint(pos.x + 40, pos.y), false));              // Cout
}

void Adder4Bit::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(220, 255, 255)));

    // Draw main body
    dc.DrawRectangle(position.x, position.y, size.x, size.y);

    // Draw label
    dc.DrawText("4-bit", position.x + 15, position.y + 50);
    dc.DrawText("ADDER", position.x + 15, position.y + 65);

    // Draw pin labels
    dc.SetFont(wxFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    for (int i = 0; i < 4; i++) {
        dc.DrawText(wxString::Format("A%d", 3-i), position.x - 15, position.y + 8 + i * 20);
        dc.DrawText(wxString::Format("B%d", 3-i), position.x - 15, position.y + 88 + i * 20);
        dc.DrawText(wxString::Format("S%d", 3-i), position.x + 85, position.y + 18 + i * 20);
    }
    dc.DrawText("Cin", position.x + 25, position.y + 175);
    dc.DrawText("Cout", position.x + 20, position.y - 10);
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

void Adder4Bit::ComputeOutputs() {
    // Get 4-bit values A and B
    int aVal = 0, bVal = 0;
    for (int i = 0; i < 4; i++) {
        if (pins[i].value == LogicValue::HIGH) {
            aVal |= (1 << (3-i));
        }
        if (pins[i + 4].value == LogicValue::HIGH) {
            bVal |= (1 << (3-i));
        }
    }

    int cin = (pins[8].value == LogicValue::HIGH) ? 1 : 0;
    int result = aVal + bVal + cin;

    // Set sum outputs (S3-S0)
    for (int i = 0; i < 4; i++) {
        pins[9 + i].value = ((result >> (3-i)) & 1) ? LogicValue::HIGH : LogicValue::LOW;
    }

    // Set carry out
    pins[13].value = (result > 15) ? LogicValue::HIGH : LogicValue::LOW;
}

// Multiplexer 2-to-1 Implementation
Multiplexer2to1::Multiplexer2to1(const wxPoint& pos)
    : ArithmeticComponent(pos, wxSize(50, 60), ComponentType::MULTIPLEXER_2TO1, "MUX") {
    // Input pins: I0, I1, S
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 15), true));      // I0
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 35), true));      // I1
    pins.push_back(Pin(wxPoint(pos.x + 25, pos.y + 60), true)); // Select

    // Output pin: Y
    pins.push_back(Pin(wxPoint(pos.x + 50, pos.y + 25), false)); // Y
}

void Multiplexer2to1::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(255, 255, 220)));

    // Draw trapezoid shape for MUX
    wxPoint points[] = {
        wxPoint(position.x, position.y),
        wxPoint(position.x + 35, position.y),
        wxPoint(position.x + 50, position.y + 50),
        wxPoint(position.x, position.y + 50)
    };
    dc.DrawPolygon(4, points);

    // Draw label
    dc.DrawText("2:1", position.x + 10, position.y + 20);
    dc.DrawText("MUX", position.x + 8, position.y + 30);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("0", position.x - 8, position.y + 13);
    dc.DrawText("1", position.x - 8, position.y + 33);
    dc.DrawText("S", position.x + 22, position.y + 65);
    dc.DrawText("Y", position.x + 55, position.y + 23);
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

void Multiplexer2to1::ComputeOutputs() {
    LogicValue sel = pins[2].value;

    if (sel == LogicValue::LOW) {
        pins[3].value = pins[0].value;  // Select I0
    } else if (sel == LogicValue::HIGH) {
        pins[3].value = pins[1].value;  // Select I1
    } else {
        pins[3].value = LogicValue::UNDEFINED;
    }
}

// Multiplexer 4-to-1 Implementation
Multiplexer4to1::Multiplexer4to1(const wxPoint& pos)
    : ArithmeticComponent(pos, wxSize(60, 80), ComponentType::MULTIPLEXER_4TO1, "MUX") {
    // Input pins: I0, I1, I2, I3, S0, S1
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));      // I0
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 25), true));      // I1
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 40), true));      // I2
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 55), true));      // I3
    pins.push_back(Pin(wxPoint(pos.x + 15, pos.y + 80), true)); // S0
    pins.push_back(Pin(wxPoint(pos.x + 35, pos.y + 80), true)); // S1

    // Output pin: Y
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 32), false)); // Y
}

void Multiplexer4to1::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(255, 255, 220)));

    // Draw trapezoid shape for MUX
    wxPoint points[] = {
        wxPoint(position.x, position.y),
        wxPoint(position.x + 45, position.y),
        wxPoint(position.x + 60, position.y + 65),
        wxPoint(position.x, position.y + 65)
    };
    dc.DrawPolygon(4, points);

    // Draw label
    dc.DrawText("4:1", position.x + 15, position.y + 25);
    dc.DrawText("MUX", position.x + 12, position.y + 38);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("0", position.x - 8, position.y + 8);
    dc.DrawText("1", position.x - 8, position.y + 23);
    dc.DrawText("2", position.x - 8, position.y + 38);
    dc.DrawText("3", position.x - 8, position.y + 53);
    dc.DrawText("S0", position.x + 10, position.y + 85);
    dc.DrawText("S1", position.x + 30, position.y + 85);
    dc.DrawText("Y", position.x + 65, position.y + 30);
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

void Multiplexer4to1::ComputeOutputs() {
    LogicValue s0 = pins[4].value;
    LogicValue s1 = pins[5].value;

    int sel = 0;
    if (s0 == LogicValue::HIGH) sel |= 1;
    if (s1 == LogicValue::HIGH) sel |= 2;

    if (s0 != LogicValue::UNDEFINED && s1 != LogicValue::UNDEFINED) {
        pins[6].value = pins[sel].value;
    } else {
        pins[6].value = LogicValue::UNDEFINED;
    }
}

// Demultiplexer 1-to-2 Implementation
Demultiplexer1to2::Demultiplexer1to2(const wxPoint& pos)
    : ArithmeticComponent(pos, wxSize(50, 60), ComponentType::DEMULTIPLEXER_1TO2, "DEMUX") {
    // Input pins: I, S
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 25), true));      // I
    pins.push_back(Pin(wxPoint(pos.x + 25, pos.y + 60), true)); // Select

    // Output pins: Y0, Y1
    pins.push_back(Pin(wxPoint(pos.x + 50, pos.y + 15), false)); // Y0
    pins.push_back(Pin(wxPoint(pos.x + 50, pos.y + 35), false)); // Y1
}

void Demultiplexer1to2::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(255, 220, 255)));

    // Draw trapezoid shape for DEMUX (inverted)
    wxPoint points[] = {
        wxPoint(position.x, position.y),
        wxPoint(position.x + 35, position.y),
        wxPoint(position.x + 50, position.y + 50),
        wxPoint(position.x + 15, position.y + 50)
    };
    dc.DrawPolygon(4, points);

    // Draw label
    dc.DrawText("1:2", position.x + 8, position.y + 20);
    dc.DrawText("DMX", position.x + 5, position.y + 30);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("I", position.x - 8, position.y + 23);
    dc.DrawText("S", position.x + 22, position.y + 65);
    dc.DrawText("0", position.x + 55, position.y + 13);
    dc.DrawText("1", position.x + 55, position.y + 33);
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

void Demultiplexer1to2::ComputeOutputs() {
    LogicValue input = pins[0].value;
    LogicValue sel = pins[1].value;

    if (sel == LogicValue::LOW) {
        pins[2].value = input;          // Y0 = input
        pins[3].value = LogicValue::LOW; // Y1 = 0
    } else if (sel == LogicValue::HIGH) {
        pins[2].value = LogicValue::LOW; // Y0 = 0
        pins[3].value = input;          // Y1 = input
    } else {
        pins[2].value = LogicValue::UNDEFINED;
        pins[3].value = LogicValue::UNDEFINED;
    }
}

// Demultiplexer 1-to-4 Implementation
Demultiplexer1to4::Demultiplexer1to4(const wxPoint& pos)
    : ArithmeticComponent(pos, wxSize(60, 80), ComponentType::DEMULTIPLEXER_1TO4, "DEMUX") {
    // Input pins: I, S0, S1
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 32), true));      // I
    pins.push_back(Pin(wxPoint(pos.x + 15, pos.y + 80), true)); // S0
    pins.push_back(Pin(wxPoint(pos.x + 35, pos.y + 80), true)); // S1

    // Output pins: Y0, Y1, Y2, Y3
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 10), false)); // Y0
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 25), false)); // Y1
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 40), false)); // Y2
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 55), false)); // Y3
}

void Demultiplexer1to4::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(255, 220, 255)));

    // Draw trapezoid shape for DEMUX (inverted)
    wxPoint points[] = {
        wxPoint(position.x, position.y),
        wxPoint(position.x + 45, position.y),
        wxPoint(position.x + 60, position.y + 65),
        wxPoint(position.x + 15, position.y + 65)
    };
    dc.DrawPolygon(4, points);

    // Draw label
    dc.DrawText("1:4", position.x + 12, position.y + 25);
    dc.DrawText("DMX", position.x + 8, position.y + 38);

    // Draw pin labels
    dc.SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("I", position.x - 8, position.y + 30);
    dc.DrawText("S0", position.x + 10, position.y + 85);
    dc.DrawText("S1", position.x + 30, position.y + 85);
    dc.DrawText("0", position.x + 65, position.y + 8);
    dc.DrawText("1", position.x + 65, position.y + 23);
    dc.DrawText("2", position.x + 65, position.y + 38);
    dc.DrawText("3", position.x + 65, position.y + 53);
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

void Demultiplexer1to4::ComputeOutputs() {
    LogicValue input = pins[0].value;
    LogicValue s0 = pins[1].value;
    LogicValue s1 = pins[2].value;

    int sel = 0;
    if (s0 == LogicValue::HIGH) sel |= 1;
    if (s1 == LogicValue::HIGH) sel |= 2;

    // Initialize all outputs to LOW
    for (int i = 3; i < 7; i++) {
        pins[i].value = LogicValue::LOW;
    }

    // Set selected output to input value
    if (s0 != LogicValue::UNDEFINED && s1 != LogicValue::UNDEFINED) {
        pins[3 + sel].value = input;
    } else {
        // If select lines are undefined, all outputs are undefined
        for (int i = 3; i < 7; i++) {
            pins[i].value = LogicValue::UNDEFINED;
        }
    }
}