#pragma once
#include "circuit_component.h"
#include <vector>

// Wire for connecting components
class Wire : public CircuitComponent {
private:
    Pin* startPin;
    Pin* endPin;
    std::vector<wxPoint> points;

public:
    Wire(Pin* start);
    void SetEndPin(Pin* end);
    void AddPoint(const wxPoint& pt);
    void Draw(wxDC& dc) override;
    bool Contains(const wxPoint& pt) const override;
    void Move(const wxPoint& offset) override;

    Pin* GetStartPin() const { return startPin; }
    Pin* GetEndPin() const { return endPin; }
};