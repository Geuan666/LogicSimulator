#pragma once
#include "circuit_component.h"

// Input Switch component
class InputSwitch : public CircuitComponent {
private:
    bool state;

public:
    InputSwitch(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    void Toggle();
    LogicValue GetValue() const;
};

// Output LED component
class OutputLED : public CircuitComponent {
public:
    OutputLED(const wxPoint& pos);
    void Draw(wxDC& dc) override;
};