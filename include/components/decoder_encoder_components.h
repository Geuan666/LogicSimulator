#pragma once
#include "circuit_component.h"

// 3-to-8 Line Decoder
class Decoder3to8 : public CircuitComponent {
public:
    Decoder3to8(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void ComputeOutputs();
    LogicValue GetOutput(int index) const;
};

// 4-to-16 Line Decoder
class Decoder4to16 : public CircuitComponent {
public:
    Decoder4to16(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void ComputeOutputs();
    LogicValue GetOutput(int index) const;
};

// BCD to 7-Segment Decoder
class BCDTo7SegmentDecoder : public CircuitComponent {
private:
    bool outputs[7]; // a, b, c, d, e, f, g segments

public:
    BCDTo7SegmentDecoder(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void ComputeOutputs();
    bool GetSegment(int segment) const; // 0-6 for a-g
};

// 8-to-3 Priority Encoder
class PriorityEncoder8to3 : public CircuitComponent {
public:
    PriorityEncoder8to3(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void ComputeOutputs();
    LogicValue GetOutput(int index) const;
    bool IsValidInput() const;
};

// 4-to-2 Priority Encoder
class PriorityEncoder4to2 : public CircuitComponent {
public:
    PriorityEncoder4to2(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void ComputeOutputs();
    LogicValue GetOutput(int index) const;
    bool IsValidInput() const;
};

// Magnitude Comparator (4-bit)
class MagnitudeComparator4Bit : public CircuitComponent {
public:
    MagnitudeComparator4Bit(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void ComputeOutputs();
    LogicValue GetAGreaterB() const;
    LogicValue GetAEqualB() const;
    LogicValue GetALessB() const;
};

// Parity Generator/Checker (4-bit)
class ParityGenerator4Bit : public CircuitComponent {
private:
    bool evenParity; // true for even parity, false for odd parity

public:
    ParityGenerator4Bit(const wxPoint& pos, bool even = true);

    void Draw(wxDC& dc) override;
    void ComputeOutputs();
    void SetParityType(bool even) { evenParity = even; }
    LogicValue GetParityOutput() const;
};