#include "../../include/components/logic_gates.h"

LogicGate::LogicGate(const wxPoint& pos, const wxSize& sz, ComponentType t, const std::string& lbl)
    : CircuitComponent(pos, sz, t), label(lbl) {
}

// AND Gate
AndGate::AndGate(const wxPoint& pos)
    : LogicGate(pos, wxSize(60, 40), ComponentType::AND_GATE, "AND") {
    // Create input pins
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 30), true));
    // Create output pin
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 20), false));
}

void AndGate::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 240, 240)));

    // Draw the gate body
    wxPoint points[6];
    points[0] = wxPoint(position.x, position.y);
    points[1] = wxPoint(position.x + size.x / 2, position.y);
    points[2] = wxPoint(position.x + size.x, position.y + size.y / 2);
    points[3] = wxPoint(position.x + size.x / 2, position.y + size.y);
    points[4] = wxPoint(position.x, position.y + size.y);
    points[5] = wxPoint(position.x, position.y);

    dc.DrawPolygon(6, points);

    // Draw the label
    dc.DrawText(label, position.x + 10, position.y + size.y / 2 - 10);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

LogicValue AndGate::Evaluate() {
    if (pins[0].value == LogicValue::HIGH && pins[1].value == LogicValue::HIGH) {
        return LogicValue::HIGH;
    }
    return LogicValue::LOW;
}

// OR Gate
OrGate::OrGate(const wxPoint& pos)
    : LogicGate(pos, wxSize(60, 40), ComponentType::OR_GATE, "OR") {
    // Create input pins
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 30), true));
    // Create output pin
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 20), false));
}

void OrGate::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 240, 240)));

    // Draw the gate body
    wxPoint points[7];
    points[0] = wxPoint(position.x, position.y);
    points[1] = wxPoint(position.x + size.x / 2, position.y);
    points[2] = wxPoint(position.x + size.x, position.y + size.y / 2);
    points[3] = wxPoint(position.x + size.x / 2, position.y + size.y);
    points[4] = wxPoint(position.x, position.y + size.y);
    points[5] = wxPoint(position.x + 10, position.y + size.y / 2);
    points[6] = wxPoint(position.x, position.y);

    dc.DrawPolygon(7, points);

    // Draw the label
    dc.DrawText(label, position.x + 10, position.y + size.y / 2 - 10);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

LogicValue OrGate::Evaluate() {
    if (pins[0].value == LogicValue::HIGH || pins[1].value == LogicValue::HIGH) {
        return LogicValue::HIGH;
    }
    return LogicValue::LOW;
}

// NOT Gate
NotGate::NotGate(const wxPoint& pos)
    : LogicGate(pos, wxSize(60, 40), ComponentType::NOT_GATE, "NOT") {
    // Create input pin
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 20), true));
    // Create output pin
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 20), false));
}

void NotGate::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 240, 240)));

    // Draw the gate body (triangle)
    wxPoint points[3];
    points[0] = wxPoint(position.x, position.y);
    points[1] = wxPoint(position.x + size.x - 10, position.y + size.y / 2);
    points[2] = wxPoint(position.x, position.y + size.y);

    dc.DrawPolygon(3, points);

    // Draw the bubble
    dc.DrawCircle(position.x + size.x - 5, position.y + size.y / 2, 5);

    // Draw the label
    dc.DrawText(label, position.x + 10, position.y + size.y / 2 - 10);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

LogicValue NotGate::Evaluate() {
    if (pins[0].value == LogicValue::HIGH) {
        return LogicValue::LOW;
    }
    return LogicValue::HIGH;
}

// NAND Gate
NandGate::NandGate(const wxPoint& pos)
    : LogicGate(pos, wxSize(60, 40), ComponentType::NAND_GATE, "NAND") {
    // Create input pins
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 30), true));
    // Create output pin
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 20), false));
}

void NandGate::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 240, 240)));

    // Draw the gate body (like AND gate)
    wxPoint points[6];
    points[0] = wxPoint(position.x, position.y);
    points[1] = wxPoint(position.x + size.x / 2, position.y);
    points[2] = wxPoint(position.x + size.x - 10, position.y + size.y / 2);
    points[3] = wxPoint(position.x + size.x / 2, position.y + size.y);
    points[4] = wxPoint(position.x, position.y + size.y);
    points[5] = wxPoint(position.x, position.y);

    dc.DrawPolygon(6, points);

    // Draw the bubble (for NAND)
    dc.DrawCircle(position.x + size.x - 5, position.y + size.y / 2, 5);

    // Draw the label
    dc.DrawText(label, position.x + 5, position.y + size.y / 2 - 10);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

LogicValue NandGate::Evaluate() {
    if (pins[0].value == LogicValue::HIGH && pins[1].value == LogicValue::HIGH) {
        return LogicValue::LOW;
    }
    return LogicValue::HIGH;
}

// NOR Gate
NorGate::NorGate(const wxPoint& pos)
    : LogicGate(pos, wxSize(60, 40), ComponentType::NOR_GATE, "NOR") {
    // Create input pins
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 30), true));
    // Create output pin
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 20), false));
}

void NorGate::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 240, 240)));

    // Draw the gate body (like OR gate)
    wxPoint points[7];
    points[0] = wxPoint(position.x, position.y);
    points[1] = wxPoint(position.x + size.x / 2, position.y);
    points[2] = wxPoint(position.x + size.x - 10, position.y + size.y / 2);
    points[3] = wxPoint(position.x + size.x / 2, position.y + size.y);
    points[4] = wxPoint(position.x, position.y + size.y);
    points[5] = wxPoint(position.x + 10, position.y + size.y / 2);
    points[6] = wxPoint(position.x, position.y);

    dc.DrawPolygon(7, points);

    // Draw the bubble (for NOR)
    dc.DrawCircle(position.x + size.x - 5, position.y + size.y / 2, 5);

    // Draw the label
    dc.DrawText(label, position.x + 5, position.y + size.y / 2 - 10);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

LogicValue NorGate::Evaluate() {
    if (pins[0].value == LogicValue::HIGH || pins[1].value == LogicValue::HIGH) {
        return LogicValue::LOW;
    }
    return LogicValue::HIGH;
}

// XOR Gate
XorGate::XorGate(const wxPoint& pos)
    : LogicGate(pos, wxSize(60, 40), ComponentType::XOR_GATE, "XOR") {
    // Create input pins
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 30), true));
    // Create output pin
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 20), false));
}

void XorGate::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 240, 240)));

    // Draw the gate body (like OR gate but with an extra curve)
    wxPoint points[7];
    points[0] = wxPoint(position.x + 5, position.y);
    points[1] = wxPoint(position.x + size.x / 2, position.y);
    points[2] = wxPoint(position.x + size.x, position.y + size.y / 2);
    points[3] = wxPoint(position.x + size.x / 2, position.y + size.y);
    points[4] = wxPoint(position.x + 5, position.y + size.y);
    points[5] = wxPoint(position.x + 15, position.y + size.y / 2);
    points[6] = wxPoint(position.x + 5, position.y);

    dc.DrawPolygon(7, points);

    // Draw the extra curve to represent XOR
    dc.DrawArc(points[4].x - 5, points[4].y, points[0].x - 5, points[0].y, points[5].x - 5, points[5].y);

    // Draw the label
    dc.DrawText(label, position.x + 10, position.y + size.y / 2 - 10);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

LogicValue XorGate::Evaluate() {
    if ((pins[0].value == LogicValue::HIGH && pins[1].value == LogicValue::LOW) ||
        (pins[0].value == LogicValue::LOW && pins[1].value == LogicValue::HIGH)) {
        return LogicValue::HIGH;
    }
    return LogicValue::LOW;
}

// XNOR Gate
XnorGate::XnorGate(const wxPoint& pos)
    : LogicGate(pos, wxSize(60, 40), ComponentType::XNOR_GATE, "XNOR") {
    // Create input pins
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 10), true));
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 30), true));
    // Create output pin
    pins.push_back(Pin(wxPoint(pos.x + 60, pos.y + 20), false));
}

void XnorGate::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(240, 240, 240)));

    // Draw the gate body (like XOR gate)
    wxPoint points[7];
    points[0] = wxPoint(position.x + 5, position.y);
    points[1] = wxPoint(position.x + size.x / 2, position.y);
    points[2] = wxPoint(position.x + size.x - 10, position.y + size.y / 2);
    points[3] = wxPoint(position.x + size.x / 2, position.y + size.y);
    points[4] = wxPoint(position.x + 5, position.y + size.y);
    points[5] = wxPoint(position.x + 15, position.y + size.y / 2);
    points[6] = wxPoint(position.x + 5, position.y);

    dc.DrawPolygon(7, points);

    // Draw the extra curve to represent XOR part
    dc.DrawArc(points[4].x - 5, points[4].y, points[0].x - 5, points[0].y, points[5].x - 5, points[5].y);

    // Draw the bubble (for XNOR)
    dc.DrawCircle(position.x + size.x - 5, position.y + size.y / 2, 5);

    // Draw the label
    dc.DrawText(label, position.x + 5, position.y + size.y / 2 - 10);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

LogicValue XnorGate::Evaluate() {
    if ((pins[0].value == LogicValue::HIGH && pins[1].value == LogicValue::HIGH) ||
        (pins[0].value == LogicValue::LOW && pins[1].value == LogicValue::LOW)) {
        return LogicValue::HIGH;
    }
    return LogicValue::LOW;
}