#pragma once
#include "circuit_component.h"

// Seven-Segment Display
class SevenSegmentDisplay : public CircuitComponent {
private:
    bool segments[7]; // a, b, c, d, e, f, g
    bool commonCathode; // true for common cathode, false for common anode
    wxColour onColor;
    wxColour offColor;

public:
    SevenSegmentDisplay(const wxPoint& pos, bool commonCathode = true);

    void Draw(wxDC& dc) override;
    void UpdateDisplay();
    void SetSegment(int segment, bool state);
    bool GetSegment(int segment) const;
    void SetCommonCathode(bool cathode) { commonCathode = cathode; }
    void SetColors(const wxColour& on, const wxColour& off);
};

// LED Matrix 8x8
class LEDMatrix8x8 : public CircuitComponent {
private:
    bool leds[8][8];
    wxColour onColor;
    wxColour offColor;
    int currentRow; // for scanning
    int currentCol;

public:
    LEDMatrix8x8(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateMatrix();
    void SetLED(int row, int col, bool state);
    bool GetLED(int row, int col) const;
    void SetColors(const wxColour& on, const wxColour& off);
    void Clear();
    void SetPattern(const bool pattern[8][8]);
};

// Simple LCD Display (16x2 character)
class LCDDisplay16x2 : public CircuitComponent {
private:
    char displayData[2][16]; // 2 rows, 16 columns
    int currentRow;
    int currentCol;
    bool cursorVisible;
    bool displayOn;
    wxColour backgroundColor;
    wxColour textColor;

public:
    LCDDisplay16x2(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateDisplay();
    void WriteCharacter(char c);
    void SetCursor(int row, int col);
    void Clear();
    void SetDisplayOn(bool on) { displayOn = on; }
    void SetCursorVisible(bool visible) { cursorVisible = visible; }
    void WriteString(const wxString& text, int row = 0, int col = 0);
};

// Hexadecimal Display (shows 4-bit value as hex)
class HexDisplay : public CircuitComponent {
private:
    int value; // 0-15
    wxColour backgroundColor;
    wxColour textColor;

public:
    HexDisplay(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateValue();
    void SetValue(int val) { value = val & 0xF; } // Limit to 4 bits
    int GetValue() const { return value; }
    char GetHexChar() const;
};

// Binary Display (shows individual bits)
class BinaryDisplay8Bit : public CircuitComponent {
private:
    bool bits[8];
    wxColour onColor;
    wxColour offColor;

public:
    BinaryDisplay8Bit(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void UpdateBits();
    void SetBit(int bit, bool state);
    bool GetBit(int bit) const;
    void SetValue(int value);
    int GetValue() const;
};

// Oscilloscope-like waveform display
class WaveformDisplay : public CircuitComponent {
private:
    std::vector<LogicValue> waveformData;
    size_t maxSamples;
    wxColour waveColor;
    wxColour backgroundColor;
    double timeScale;

public:
    WaveformDisplay(const wxPoint& pos);

    void Draw(wxDC& dc) override;
    void AddSample(LogicValue value);
    void Clear();
    void SetTimeScale(double scale) { timeScale = scale; }
    void SetMaxSamples(size_t max);
};