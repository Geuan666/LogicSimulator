#pragma once
#include "circuit_component.h"

// Clock edge detection
enum class ClockEdge {
    RISING,
    FALLING,
    BOTH
};

// Base class for sequential logic components
class SequentialComponent : public CircuitComponent {
protected:
    LogicValue lastClockValue;
    ClockEdge triggerEdge;
    bool hasClockInput;

public:
    SequentialComponent(const wxPoint& pos, const wxSize& sz, ComponentType t);

    virtual void UpdateOnClock() = 0;
    virtual bool IsClockTriggered(LogicValue newClockValue);
    void SetTriggerEdge(ClockEdge edge) { triggerEdge = edge; }
    ClockEdge GetTriggerEdge() const { return triggerEdge; }
};

// D Flip-Flop
class DFlipFlop : public SequentialComponent {
private:
    LogicValue outputQ;
    LogicValue outputQNot;

public:
    DFlipFlop(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateOnClock() override;
    LogicValue GetQ() const { return outputQ; }
    LogicValue GetQNot() const { return outputQNot; }
};

// JK Flip-Flop
class JKFlipFlop : public SequentialComponent {
private:
    LogicValue outputQ;
    LogicValue outputQNot;

public:
    JKFlipFlop(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateOnClock() override;
    LogicValue GetQ() const { return outputQ; }
    LogicValue GetQNot() const { return outputQNot; }
};

// SR Latch (Set-Reset)
class SRLatch : public SequentialComponent {
private:
    LogicValue outputQ;
    LogicValue outputQNot;

public:
    SRLatch(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateOnClock() override;
    LogicValue GetQ() const { return outputQ; }
    LogicValue GetQNot() const { return outputQNot; }
};

// T Flip-Flop (Toggle)
class TFlipFlop : public SequentialComponent {
private:
    LogicValue outputQ;
    LogicValue outputQNot;

public:
    TFlipFlop(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateOnClock() override;
    LogicValue GetQ() const { return outputQ; }
    LogicValue GetQNot() const { return outputQNot; }
};

// Clock Generator
class ClockGenerator : public CircuitComponent {
private:
    bool clockState;
    int frequency; // Hz
    wxLongLong lastToggleTime;

public:
    ClockGenerator(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void Update();
    void SetFrequency(int freq) { frequency = freq; }
    int GetFrequency() const { return frequency; }
    LogicValue GetClockOutput();
};

// 4-bit Register
class Register4Bit : public SequentialComponent {
private:
    LogicValue data[4];
    LogicValue outputs[4];

public:
    Register4Bit(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateOnClock() override;
    void SetData(int index, LogicValue value);
    LogicValue GetOutput(int index) const;
};

// 4-bit Shift Register
class ShiftRegister4Bit : public SequentialComponent {
private:
    LogicValue registers[4];
    bool shiftLeft; // true for left shift, false for right shift

public:
    ShiftRegister4Bit(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateOnClock() override;
    void SetShiftDirection(bool left) { shiftLeft = left; }
    LogicValue GetOutput(int index) const;
};

// 4-bit Binary Counter
class BinaryCounter4Bit : public SequentialComponent {
private:
    int count;
    bool countUp; // true for up counter, false for down counter

public:
    BinaryCounter4Bit(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateOnClock() override;
    void SetCountDirection(bool up) { countUp = up; }
    void Reset() { count = 0; }
    int GetCount() const { return count; }
    LogicValue GetOutput(int bit) const;
};

// BCD Counter (0-9)
class BCDCounter : public SequentialComponent {
private:
    int count;

public:
    BCDCounter(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateOnClock() override;
    void Reset() { count = 0; }
    int GetCount() const { return count; }
    LogicValue GetOutput(int bit) const;
};