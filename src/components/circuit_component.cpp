#include "../../include/components/circuit_component.h"
#include <cmath>
#include <algorithm>

CircuitComponent::CircuitComponent(const wxPoint& pos, const wxSize& sz, ComponentType t)
    : position(pos), size(sz), selected(false), type(t), rotation(0.0), scaleX(1.0), scaleY(1.0) {
}

void CircuitComponent::Move(const wxPoint& offset) {
    position += offset;
    for (auto& pin : pins) {
        pin.position += offset;
    }
}

bool CircuitComponent::Contains(const wxPoint& pt) const {
    return (pt.x >= position.x && pt.x <= position.x + size.x &&
            pt.y >= position.y && pt.y <= position.y + size.y);
}

Pin* CircuitComponent::GetPinAt(const wxPoint& pt) {
    for (auto& pin : pins) {
        wxRect pinRect(pin.position.x - 5, pin.position.y - 5, 10, 10);
        if (pinRect.Contains(pt)) {
            return &pin;
        }
    }
    return nullptr;
}

void CircuitComponent::SetRotation(double degrees) {
    // Normalize to 0, 90, 180, 270 degrees for now
    rotation = fmod(degrees, 360.0);
    if (rotation < 0) rotation += 360.0;

    // Snap to 90-degree increments
    double remainder = fmod(rotation, 90.0);
    if (remainder < 45.0) {
        rotation = rotation - remainder;
    } else {
        rotation = rotation + (90.0 - remainder);
    }
    if (rotation >= 360.0) rotation = 0.0;
}

void CircuitComponent::Rotate(double degrees) {
    SetRotation(rotation + degrees);
}

void CircuitComponent::SetScale(double sx, double sy) {
    // Limit scale factors to reasonable ranges
    scaleX = std::max(0.25, std::min(4.0, sx));
    scaleY = std::max(0.25, std::min(4.0, sy));
}

void CircuitComponent::Scale(double sx, double sy) {
    SetScale(scaleX * sx, scaleY * sy);
}

wxSize CircuitComponent::GetEffectiveSize() const {
    wxSize effectiveSize = size;

    // Apply scaling
    effectiveSize.x = static_cast<int>(effectiveSize.x * scaleX);
    effectiveSize.y = static_cast<int>(effectiveSize.y * scaleY);

    // For 90 or 270 degree rotations, swap width and height
    if (rotation == 90.0 || rotation == 270.0) {
        int temp = effectiveSize.x;
        effectiveSize.x = effectiveSize.y;
        effectiveSize.y = temp;
    }

    return effectiveSize;
}