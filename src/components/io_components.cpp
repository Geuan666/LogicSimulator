#include "../../include/components/io_components.h"

// Input Switch Implementation
InputSwitch::InputSwitch(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(40, 30), ComponentType::INPUT_PIN), state(false) {
    // Create output pin
    pins.push_back(Pin(wxPoint(pos.x + 40, pos.y + 15), false));
}

void InputSwitch::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);
    dc.SetBrush(state ? *wxGREEN_BRUSH : *wxWHITE_BRUSH);

    // Draw the switch body
    dc.DrawRectangle(position.x, position.y, size.x, size.y);

    // Draw the state text
    dc.DrawText(state ? "ON" : "OFF", position.x + 5, position.y + 5);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}

void InputSwitch::Toggle() {
    state = !state;
    pins[0].value = state ? LogicValue::HIGH : LogicValue::LOW;
}

LogicValue InputSwitch::GetValue() const {
    return state ? LogicValue::HIGH : LogicValue::LOW;
}

// Output LED Implementation
OutputLED::OutputLED(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(30, 30), ComponentType::OUTPUT_PIN) {
    // Create input pin
    pins.push_back(Pin(wxPoint(pos.x, pos.y + 15), true));
}

void OutputLED::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);

    // Set LED color based on input value
    if (pins[0].value == LogicValue::HIGH) {
        dc.SetBrush(*wxGREEN_BRUSH);
    } else if (pins[0].value == LogicValue::LOW) {
        dc.SetBrush(*wxWHITE_BRUSH);
    } else {
        dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    }

    // Draw the LED body
    dc.DrawCircle(position.x + size.x / 2, position.y + size.y / 2, size.x / 2);

    // Draw pins
    for (const auto& pin : pins) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(pin.isConnected ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
        dc.DrawCircle(pin.position, 3);
    }
}