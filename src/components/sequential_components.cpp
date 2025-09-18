#include "../../include/components/sequential_components.h"
#include <wx/dc.h>

// SequentialComponent base class
SequentialComponent::SequentialComponent(const wxPoint& pos, const wxSize& sz, ComponentType t)
    : CircuitComponent(pos, sz, t), lastClockValue(LogicValue::UNDEFINED),
      triggerEdge(ClockEdge::RISING), hasClockInput(true) {
}

bool SequentialComponent::IsClockTriggered(LogicValue newClockValue) {
    bool triggered = false;

    if (triggerEdge == ClockEdge::RISING) {
        triggered = (lastClockValue == LogicValue::LOW && newClockValue == LogicValue::HIGH);
    } else if (triggerEdge == ClockEdge::FALLING) {
        triggered = (lastClockValue == LogicValue::HIGH && newClockValue == LogicValue::LOW);
    } else { // BOTH
        triggered = (lastClockValue != newClockValue &&
                    (newClockValue == LogicValue::HIGH || newClockValue == LogicValue::LOW));
    }

    lastClockValue = newClockValue;
    return triggered;
}

// D Flip-Flop implementation
DFlipFlop::DFlipFlop(const wxPoint& pos)
    : SequentialComponent(pos, wxSize(60, 80), ComponentType::D_FLIPFLOP),
      outputQ(LogicValue::LOW), outputQNot(LogicValue::HIGH) {

    // D input, Clock input, Q output, Q̄ output
    pins.emplace_back(wxPoint(pos.x, pos.y + 20), true);      // D
    pins.emplace_back(wxPoint(pos.x, pos.y + 60), true);      // Clock
    pins.emplace_back(wxPoint(pos.x + 60, pos.y + 20), false); // Q
    pins.emplace_back(wxPoint(pos.x + 60, pos.y + 60), false); // Q̄
}

void DFlipFlop::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxWHITE));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw label
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.DrawText("D FF", pos.x + 15, pos.y + 30);

    // Draw pin labels
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("D", pos.x - 15, pos.y + 15);
    dc.DrawText("CLK", pos.x - 20, pos.y + 55);
    dc.DrawText("Q", pos.x + 65, pos.y + 15);
    dc.DrawText("Q̄", pos.x + 65, pos.y + 55);

    // Draw clock triangle
    wxPoint triangle[] = {
        wxPoint(pos.x + 5, pos.y + 65),
        wxPoint(pos.x + 15, pos.y + 60),
        wxPoint(pos.x + 5, pos.y + 55)
    };
    dc.DrawPolygon(3, triangle);

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

void DFlipFlop::UpdateOnClock() {
    if (pins.size() >= 4) {
        LogicValue clockValue = pins[1].value; // Clock input

        if (IsClockTriggered(clockValue)) {
            LogicValue dInput = pins[0].value; // D input
            outputQ = dInput;
            outputQNot = (dInput == LogicValue::HIGH) ? LogicValue::LOW :
                        (dInput == LogicValue::LOW) ? LogicValue::HIGH : LogicValue::UNDEFINED;

            pins[2].value = outputQ;     // Q output
            pins[3].value = outputQNot;  // Q̄ output
        }
    }
}

// JK Flip-Flop implementation
JKFlipFlop::JKFlipFlop(const wxPoint& pos)
    : SequentialComponent(pos, wxSize(60, 100), ComponentType::JK_FLIPFLOP),
      outputQ(LogicValue::LOW), outputQNot(LogicValue::HIGH) {

    // J input, K input, Clock input, Q output, Q̄ output
    pins.emplace_back(wxPoint(pos.x, pos.y + 15), true);      // J
    pins.emplace_back(wxPoint(pos.x, pos.y + 45), true);      // K
    pins.emplace_back(wxPoint(pos.x, pos.y + 75), true);      // Clock
    pins.emplace_back(wxPoint(pos.x + 60, pos.y + 25), false); // Q
    pins.emplace_back(wxPoint(pos.x + 60, pos.y + 65), false); // Q̄
}

void JKFlipFlop::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxWHITE));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw label
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.DrawText("JK FF", pos.x + 10, pos.y + 40);

    // Draw pin labels
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("J", pos.x - 15, pos.y + 10);
    dc.DrawText("K", pos.x - 15, pos.y + 40);
    dc.DrawText("CLK", pos.x - 20, pos.y + 70);
    dc.DrawText("Q", pos.x + 65, pos.y + 20);
    dc.DrawText("Q̄", pos.x + 65, pos.y + 60);

    // Draw clock triangle
    wxPoint triangle[] = {
        wxPoint(pos.x + 5, pos.y + 80),
        wxPoint(pos.x + 15, pos.y + 75),
        wxPoint(pos.x + 5, pos.y + 70)
    };
    dc.DrawPolygon(3, triangle);

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

void JKFlipFlop::UpdateOnClock() {
    if (pins.size() >= 5) {
        LogicValue clockValue = pins[2].value; // Clock input

        if (IsClockTriggered(clockValue)) {
            LogicValue jInput = pins[0].value; // J input
            LogicValue kInput = pins[1].value; // K input

            // JK Flip-flop truth table
            if (jInput == LogicValue::LOW && kInput == LogicValue::LOW) {
                // No change
            } else if (jInput == LogicValue::LOW && kInput == LogicValue::HIGH) {
                outputQ = LogicValue::LOW;
                outputQNot = LogicValue::HIGH;
            } else if (jInput == LogicValue::HIGH && kInput == LogicValue::LOW) {
                outputQ = LogicValue::HIGH;
                outputQNot = LogicValue::LOW;
            } else if (jInput == LogicValue::HIGH && kInput == LogicValue::HIGH) {
                // Toggle
                LogicValue newQ = (outputQ == LogicValue::HIGH) ? LogicValue::LOW : LogicValue::HIGH;
                outputQ = newQ;
                outputQNot = (newQ == LogicValue::HIGH) ? LogicValue::LOW : LogicValue::HIGH;
            }

            pins[3].value = outputQ;     // Q output
            pins[4].value = outputQNot;  // Q̄ output
        }
    }
}

// SR Latch implementation
SRLatch::SRLatch(const wxPoint& pos)
    : SequentialComponent(pos, wxSize(60, 80), ComponentType::SR_LATCH),
      outputQ(LogicValue::LOW), outputQNot(LogicValue::HIGH) {

    // S input, R input, Q output, Q̄ output
    pins.emplace_back(wxPoint(pos.x, pos.y + 20), true);      // S (Set)
    pins.emplace_back(wxPoint(pos.x, pos.y + 60), true);      // R (Reset)
    pins.emplace_back(wxPoint(pos.x + 60, pos.y + 20), false); // Q
    pins.emplace_back(wxPoint(pos.x + 60, pos.y + 60), false); // Q̄

    hasClockInput = false; // SR Latch is level-triggered, not edge-triggered
}

void SRLatch::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxWHITE));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw label
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.DrawText("SR", pos.x + 20, pos.y + 30);

    // Draw pin labels
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText("S", pos.x - 15, pos.y + 15);
    dc.DrawText("R", pos.x - 15, pos.y + 55);
    dc.DrawText("Q", pos.x + 65, pos.y + 15);
    dc.DrawText("Q̄", pos.x + 65, pos.y + 55);

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

void SRLatch::UpdateOnClock() {
    if (pins.size() >= 4) {
        LogicValue sInput = pins[0].value; // S input
        LogicValue rInput = pins[1].value; // R input

        // SR Latch truth table (level-triggered)
        if (sInput == LogicValue::LOW && rInput == LogicValue::LOW) {
            // Hold current state
        } else if (sInput == LogicValue::LOW && rInput == LogicValue::HIGH) {
            outputQ = LogicValue::LOW;
            outputQNot = LogicValue::HIGH;
        } else if (sInput == LogicValue::HIGH && rInput == LogicValue::LOW) {
            outputQ = LogicValue::HIGH;
            outputQNot = LogicValue::LOW;
        } else if (sInput == LogicValue::HIGH && rInput == LogicValue::HIGH) {
            // Invalid state - both outputs go low (not allowed in real circuits)
            outputQ = LogicValue::UNDEFINED;
            outputQNot = LogicValue::UNDEFINED;
        }

        pins[2].value = outputQ;     // Q output
        pins[3].value = outputQNot;  // Q̄ output
    }
}

// Clock Generator implementation
ClockGenerator::ClockGenerator(const wxPoint& pos)
    : CircuitComponent(pos, wxSize(60, 40), ComponentType::CLOCK_GENERATOR),
      clockState(false), frequency(1), lastToggleTime(wxGetLocalTimeMillis()) {

    // Clock output
    pins.emplace_back(wxPoint(pos.x + 60, pos.y + 20), false);
}

void ClockGenerator::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(wxColour(255, 255, 0))); // Yellow background for clock
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw clock symbol (square wave)
    dc.SetPen(wxPen(*wxBLACK, 2));
    wxPoint wavePoints[] = {
        wxPoint(pos.x + 10, pos.y + 30),
        wxPoint(pos.x + 10, pos.y + 10),
        wxPoint(pos.x + 20, pos.y + 10),
        wxPoint(pos.x + 20, pos.y + 30),
        wxPoint(pos.x + 30, pos.y + 30),
        wxPoint(pos.x + 30, pos.y + 10),
        wxPoint(pos.x + 40, pos.y + 10),
        wxPoint(pos.x + 40, pos.y + 30),
        wxPoint(pos.x + 50, pos.y + 30)
    };
    dc.DrawLines(9, wavePoints);

    // Draw output pin
    dc.SetBrush(wxBrush(*wxBLACK));
    for (const auto& pin : pins) {
        wxColour pinColor = pin.isConnected ? *wxRED : *wxBLACK;
        dc.SetPen(wxPen(pinColor, 2));
        dc.DrawCircle(pin.position, 3);
    }

    // Show current state
    if (clockState) {
        dc.SetBrush(wxBrush(*wxRED));
        dc.DrawCircle(pos.x + 30, pos.y + 20, 5);
    }

    // Highlight if selected
    if (selected) {
        dc.SetPen(wxPen(*wxRED, 3, wxPENSTYLE_DOT));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(pos.x - 5, pos.y - 5, size.x + 10, size.y + 10);
    }
}

void ClockGenerator::Update() {
    wxLongLong currentTime = wxGetLocalTimeMillis();
    wxLongLong period = 1000 / frequency; // Period in milliseconds

    if (currentTime - lastToggleTime >= period / 2) {
        clockState = !clockState;
        lastToggleTime = currentTime;

        if (!pins.empty()) {
            pins[0].value = clockState ? LogicValue::HIGH : LogicValue::LOW;
        }
    }
}

LogicValue ClockGenerator::GetClockOutput() {
    return clockState ? LogicValue::HIGH : LogicValue::LOW;
}

// 4-bit Binary Counter implementation
BinaryCounter4Bit::BinaryCounter4Bit(const wxPoint& pos)
    : SequentialComponent(pos, wxSize(80, 100), ComponentType::COUNTER_4BIT),
      count(0), countUp(true) {

    // Clock input, Reset input, 4 output bits
    pins.emplace_back(wxPoint(pos.x, pos.y + 20), true);      // Clock
    pins.emplace_back(wxPoint(pos.x, pos.y + 80), true);      // Reset
    pins.emplace_back(wxPoint(pos.x + 80, pos.y + 20), false); // Q0
    pins.emplace_back(wxPoint(pos.x + 80, pos.y + 35), false); // Q1
    pins.emplace_back(wxPoint(pos.x + 80, pos.y + 50), false); // Q2
    pins.emplace_back(wxPoint(pos.x + 80, pos.y + 65), false); // Q3
}

void BinaryCounter4Bit::Draw(wxDC& dc) {
    wxPoint pos = GetPosition();
    wxSize size = GetSize();

    // Draw the main rectangle
    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.SetBrush(wxBrush(*wxWHITE));
    dc.DrawRectangle(pos.x, pos.y, size.x, size.y);

    // Draw label
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.DrawText("4-bit", pos.x + 15, pos.y + 35);
    dc.DrawText("Counter", pos.x + 10, pos.y + 50);

    // Draw current count
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText(wxString::Format("%d", count), pos.x + 35, pos.y + 10);

    // Draw pin labels
    dc.DrawText("CLK", pos.x - 20, pos.y + 15);
    dc.DrawText("RST", pos.x - 20, pos.y + 75);
    dc.DrawText("Q0", pos.x + 85, pos.y + 15);
    dc.DrawText("Q1", pos.x + 85, pos.y + 30);
    dc.DrawText("Q2", pos.x + 85, pos.y + 45);
    dc.DrawText("Q3", pos.x + 85, pos.y + 60);

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

void BinaryCounter4Bit::UpdateOnClock() {
    if (pins.size() >= 6) {
        // Check reset first
        if (pins[1].value == LogicValue::HIGH) {
            count = 0;
        } else {
            LogicValue clockValue = pins[0].value; // Clock input

            if (IsClockTriggered(clockValue)) {
                if (countUp) {
                    count = (count + 1) % 16; // 4-bit counter: 0-15
                } else {
                    count = (count - 1 + 16) % 16;
                }
            }
        }

        // Update output pins
        pins[2].value = (count & 1) ? LogicValue::HIGH : LogicValue::LOW;       // Q0
        pins[3].value = (count & 2) ? LogicValue::HIGH : LogicValue::LOW;       // Q1
        pins[4].value = (count & 4) ? LogicValue::HIGH : LogicValue::LOW;       // Q2
        pins[5].value = (count & 8) ? LogicValue::HIGH : LogicValue::LOW;       // Q3
    }
}

LogicValue BinaryCounter4Bit::GetOutput(int bit) const {
    if (bit >= 0 && bit < 4) {
        return (count & (1 << bit)) ? LogicValue::HIGH : LogicValue::LOW;
    }
    return LogicValue::UNDEFINED;
}