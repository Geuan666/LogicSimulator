#include "../../include/components/wire.h"
#include <cmath>

Wire::Wire(Pin* start)
    : CircuitComponent(start->position, wxSize(0, 0), ComponentType::WIRE), startPin(start), endPin(nullptr) {
    points.push_back(start->position);
}

void Wire::SetEndPin(Pin* end) {
    endPin = end;
    if (end) {
        points.push_back(end->position);
    }
}

void Wire::AddPoint(const wxPoint& pt) {
    points.push_back(pt);
}

void Wire::Draw(wxDC& dc) {
    dc.SetPen(selected ? *wxRED_PEN : *wxBLACK_PEN);

    // Draw the wire segments
    for (size_t i = 0; i < points.size() - 1; ++i) {
        dc.DrawLine(points[i], points[i + 1]);
    }
}

bool Wire::Contains(const wxPoint& pt) const {
    for (size_t i = 0; i < points.size() - 1; ++i) {
        // Check if point is near this line segment
        wxPoint p1 = points[i];
        wxPoint p2 = points[i + 1];

        // Calculate distance from point to line segment
        double lineLen = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
        if (lineLen < 1.0) continue; // Skip zero-length segments

        double dist = abs((p2.y - p1.y) * pt.x - (p2.x - p1.x) * pt.y + p2.x * p1.y - p2.y * p1.x) / lineLen;

        // If point is within 5 pixels of the line
        if (dist < 5.0) {
            // Check if point is between the endpoints
            double dot = ((pt.x - p1.x) * (p2.x - p1.x) + (pt.y - p1.y) * (p2.y - p1.y)) / pow(lineLen, 2);
            if (dot >= 0.0 && dot <= 1.0) {
                return true;
            }
        }
    }
    return false;
}

void Wire::Move(const wxPoint& offset) {
    // Only move if not connected to any pins
    if (!startPin && !endPin) {
        for (auto& pt : points) {
            pt += offset;
        }
    }
}