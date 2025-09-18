#pragma once
#include "circuit_component.h"
#include <string>

// Base class for arithmetic components
class ArithmeticComponent : public CircuitComponent {
protected:
    std::string label;

public:
    ArithmeticComponent(const wxPoint& pos, const wxSize& sz, ComponentType t, const std::string& lbl);
    virtual void ComputeOutputs() = 0;
};

// Half Adder - adds two single bits
class HalfAdder : public ArithmeticComponent {
public:
    HalfAdder(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    void ComputeOutputs() override;
};

// Full Adder - adds two bits plus carry in
class FullAdder : public ArithmeticComponent {
public:
    FullAdder(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    void ComputeOutputs() override;
};

// 4-bit Adder - adds two 4-bit numbers
class Adder4Bit : public ArithmeticComponent {
public:
    Adder4Bit(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    void ComputeOutputs() override;
};

// Multiplexer 2-to-1
class Multiplexer2to1 : public ArithmeticComponent {
public:
    Multiplexer2to1(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    void ComputeOutputs() override;
};

// Multiplexer 4-to-1
class Multiplexer4to1 : public ArithmeticComponent {
public:
    Multiplexer4to1(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    void ComputeOutputs() override;
};

// Demultiplexer 1-to-2
class Demultiplexer1to2 : public ArithmeticComponent {
public:
    Demultiplexer1to2(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    void ComputeOutputs() override;
};

// Demultiplexer 1-to-4
class Demultiplexer1to4 : public ArithmeticComponent {
public:
    Demultiplexer1to4(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    void ComputeOutputs() override;
};