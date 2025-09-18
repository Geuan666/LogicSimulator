#pragma once
#include "circuit_component.h"
#include <string>

// Base class for logic gates
class LogicGate : public CircuitComponent {
protected:
    std::string label;

public:
    LogicGate(const wxPoint& pos, const wxSize& sz, ComponentType t, const std::string& lbl);
    virtual LogicValue Evaluate() = 0;
};

// AND Gate
class AndGate : public LogicGate {
public:
    AndGate(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    LogicValue Evaluate() override;
};

// OR Gate
class OrGate : public LogicGate {
public:
    OrGate(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    LogicValue Evaluate() override;
};

// NOT Gate
class NotGate : public LogicGate {
public:
    NotGate(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    LogicValue Evaluate() override;
};

// NAND Gate
class NandGate : public LogicGate {
public:
    NandGate(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    LogicValue Evaluate() override;
};

// NOR Gate
class NorGate : public LogicGate {
public:
    NorGate(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    LogicValue Evaluate() override;
};

// XOR Gate
class XorGate : public LogicGate {
public:
    XorGate(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    LogicValue Evaluate() override;
};

// XNOR Gate
class XnorGate : public LogicGate {
public:
    XnorGate(const wxPoint& pos);
    void Draw(wxDC& dc) override;
    LogicValue Evaluate() override;
};