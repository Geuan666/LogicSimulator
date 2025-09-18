#pragma once
#include <wx/wx.h>
#include <vector>
#include <string>

// Enumeration for logic values
enum class LogicValue {
    LOW = 0,
    HIGH = 1,
    UNDEFINED = 2
};

// Enumeration for component types
enum class ComponentType {
    SELECT,
    INPUT_PIN,
    OUTPUT_PIN,
    AND_GATE,
    OR_GATE,
    NOT_GATE,
    NAND_GATE,
    NOR_GATE,
    XOR_GATE,
    XNOR_GATE,
    BUFFER,
    WIRE,
    // Arithmetic components
    HALF_ADDER,
    FULL_ADDER,
    ADDER_4BIT,
    // Data selection components
    MULTIPLEXER_2TO1,
    MULTIPLEXER_4TO1,
    DEMULTIPLEXER_1TO2,
    DEMULTIPLEXER_1TO4,
    // Encoder/Decoder components
    ENCODER_4TO2,
    ENCODER_8TO3,
    PRIORITY_ENCODER,
    DECODER_2TO4,
    DECODER_3TO8,
    DECODER_4TO16,
    BCD_TO_7SEGMENT,
    // Sequential logic components
    D_FLIPFLOP,
    JK_FLIPFLOP,
    SR_LATCH,
    T_FLIPFLOP,
    CLOCK_GENERATOR,
    REGISTER_4BIT,
    SHIFT_REGISTER_4BIT,
    COUNTER_4BIT,
    BCD_COUNTER,
    // Display components
    SEVEN_SEGMENT_DISPLAY,
    LED_MATRIX_8X8,
    LCD_DISPLAY,
    HEX_DISPLAY,
    BINARY_DISPLAY
};

// Connection points (pins) for components
struct Pin {
    wxPoint position;
    LogicValue value;
    bool isInput;
    bool isConnected;

    Pin(const wxPoint& pos, bool input)
        : position(pos), value(LogicValue::UNDEFINED), isInput(input), isConnected(false) {}
};

// Base class for all circuit components
class CircuitComponent {
protected:
    wxPoint position;
    wxSize size;
    std::vector<Pin> pins;
    bool selected;
    ComponentType type;

    // Transformation properties
    double rotation;    // Rotation angle in degrees (0, 90, 180, 270)
    double scaleX;      // X-axis scale factor
    double scaleY;      // Y-axis scale factor

public:
    CircuitComponent(const wxPoint& pos, const wxSize& sz, ComponentType t);
    virtual ~CircuitComponent() {}

    virtual void Draw(wxDC& dc) = 0;
    virtual void Move(const wxPoint& offset);
    virtual bool Contains(const wxPoint& pt) const;
    virtual Pin* GetPinAt(const wxPoint& pt);

    void Select(bool sel) { selected = sel; }
    bool IsSelected() const { return selected; }
    ComponentType GetType() const { return type; }
    const std::vector<Pin>& GetPins() const { return pins; }
    std::vector<Pin>& GetPins() { return pins; }
    wxPoint GetPosition() const { return position; }
    wxSize GetSize() const { return size; }

    // Transformation methods
    virtual void SetRotation(double degrees);
    virtual void Rotate(double degrees);
    virtual double GetRotation() const { return rotation; }

    virtual void SetScale(double sx, double sy);
    virtual void Scale(double sx, double sy);
    virtual void GetScale(double& sx, double& sy) const { sx = scaleX; sy = scaleY; }

    // Get effective size after transformations
    virtual wxSize GetEffectiveSize() const;
};