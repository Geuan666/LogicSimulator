#include "../../include/ui/properties_panel.h"

wxBEGIN_EVENT_TABLE(PropertiesPanel, wxPanel)
    EVT_PG_CHANGED(wxID_ANY, PropertiesPanel::OnPropertyChanged)
wxEND_EVENT_TABLE()

PropertiesPanel::PropertiesPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY), currentComponent(nullptr) {

    // Set background color
    SetBackgroundColour(wxColour(245, 245, 245));

    // Create sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Title
    titleLabel = new wxStaticText(this, wxID_ANY, "Properties");
    titleLabel->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(titleLabel, 0, wxALL | wxEXPAND, 5);

    // Separator (simplified)
    sizer->AddSpacer(5);

    // Property grid
    propGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                  wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
    propGrid->SetBackgroundColour(wxColour(255, 255, 255));
    sizer->Add(propGrid, 1, wxALL | wxEXPAND, 5);

    SetSizer(sizer);
    ClearProperties();
}

void PropertiesPanel::SetComponent(CircuitComponent* component) {
    currentComponent = component;
    if (component) {
        PopulateProperties();
        titleLabel->SetLabel(wxString::Format("Properties - %s",
                            component->GetType() == ComponentType::AND_GATE ? "AND Gate" :
                            component->GetType() == ComponentType::OR_GATE ? "OR Gate" :
                            component->GetType() == ComponentType::NOT_GATE ? "NOT Gate" :
                            component->GetType() == ComponentType::NAND_GATE ? "NAND Gate" :
                            component->GetType() == ComponentType::NOR_GATE ? "NOR Gate" :
                            component->GetType() == ComponentType::XOR_GATE ? "XOR Gate" :
                            component->GetType() == ComponentType::XNOR_GATE ? "XNOR Gate" :
                            component->GetType() == ComponentType::HALF_ADDER ? "Half Adder" :
                            component->GetType() == ComponentType::FULL_ADDER ? "Full Adder" :
                            component->GetType() == ComponentType::ADDER_4BIT ? "4-bit Adder" :
                            component->GetType() == ComponentType::MULTIPLEXER_2TO1 ? "2:1 MUX" :
                            component->GetType() == ComponentType::MULTIPLEXER_4TO1 ? "4:1 MUX" :
                            component->GetType() == ComponentType::DEMULTIPLEXER_1TO2 ? "1:2 DEMUX" :
                            component->GetType() == ComponentType::DEMULTIPLEXER_1TO4 ? "1:4 DEMUX" :
                            component->GetType() == ComponentType::INPUT_PIN ? "Input Switch" :
                            component->GetType() == ComponentType::OUTPUT_PIN ? "Output LED" :
                            "Component"));
    } else {
        ClearProperties();
    }
}

void PropertiesPanel::ClearProperties() {
    propGrid->Clear();
    titleLabel->SetLabel("Properties");

    // Add welcome message
    propGrid->Append(new wxStringProperty("Selection", "selection", "No component selected"));
    propGrid->SetPropertyReadOnly("selection");
    propGrid->SetPropertyTextColour("selection", wxColour(128, 128, 128));
}

void PropertiesPanel::PopulateProperties() {
    propGrid->Clear();

    if (!currentComponent) {
        ClearProperties();
        return;
    }

    // Basic properties for all components
    PopulateBasicProperties();

    // Type-specific properties
    PopulateTypeSpecificProperties();

    // Pin information
    PopulatePinProperties();
}

void PropertiesPanel::PopulateBasicProperties() {
    wxPGProperty* basicCat = propGrid->Append(new wxPropertyCategory("Basic Properties"));

    // Position (editable)
    wxPoint pos = currentComponent->GetPosition();
    propGrid->Append(new wxIntProperty("X Position", "pos_x", pos.x));
    propGrid->Append(new wxIntProperty("Y Position", "pos_y", pos.y));

    // Size (read-only for now)
    wxSize size = currentComponent->GetSize();
    propGrid->Append(new wxIntProperty("Width", "width", size.x));
    propGrid->Append(new wxIntProperty("Height", "height", size.y));
    propGrid->SetPropertyReadOnly("width");
    propGrid->SetPropertyReadOnly("height");

    // Component name (editable)
    propGrid->Append(new wxStringProperty("Component Name", "name", GetComponentName()));

    // Rotation (now editable)
    propGrid->Append(new wxIntProperty("Rotation (degrees)", "rotation",
                                       static_cast<int>(currentComponent->GetRotation())));

    // Scale factors
    double scaleX, scaleY;
    currentComponent->GetScale(scaleX, scaleY);
    propGrid->Append(new wxFloatProperty("Scale X", "scale_x", scaleX));
    propGrid->Append(new wxFloatProperty("Scale Y", "scale_y", scaleY));
}

void PropertiesPanel::PopulateTypeSpecificProperties() {
    ComponentType type = currentComponent->GetType();

    // Create type-specific properties category
    wxPGProperty* typeCat = propGrid->Append(new wxPropertyCategory("Component Properties"));

    switch (type) {
        case ComponentType::INPUT_PIN:
            PopulateInputProperties();
            break;
        case ComponentType::OUTPUT_PIN:
            PopulateOutputProperties();
            break;
        case ComponentType::AND_GATE:
        case ComponentType::OR_GATE:
        case ComponentType::NOT_GATE:
        case ComponentType::NAND_GATE:
        case ComponentType::NOR_GATE:
        case ComponentType::XOR_GATE:
        case ComponentType::XNOR_GATE:
        case ComponentType::BUFFER:
            PopulateLogicGateProperties();
            break;
        case ComponentType::HALF_ADDER:
        case ComponentType::FULL_ADDER:
        case ComponentType::ADDER_4BIT:
            PopulateArithmeticProperties();
            break;
        case ComponentType::MULTIPLEXER_2TO1:
        case ComponentType::MULTIPLEXER_4TO1:
        case ComponentType::DEMULTIPLEXER_1TO2:
        case ComponentType::DEMULTIPLEXER_1TO4:
            PopulateMultiplexerProperties();
            break;
        case ComponentType::ENCODER_4TO2:
        case ComponentType::ENCODER_8TO3:
        case ComponentType::PRIORITY_ENCODER:
        case ComponentType::DECODER_2TO4:
        case ComponentType::DECODER_3TO8:
        case ComponentType::DECODER_4TO16:
        case ComponentType::BCD_TO_7SEGMENT:
            PopulateDefaultProperties();
            break;
        case ComponentType::D_FLIPFLOP:
        case ComponentType::JK_FLIPFLOP:
        case ComponentType::SR_LATCH:
        case ComponentType::T_FLIPFLOP:
            PopulateSequentialProperties();
            break;
        case ComponentType::REGISTER_4BIT:
        case ComponentType::SHIFT_REGISTER_4BIT:
        case ComponentType::COUNTER_4BIT:
        case ComponentType::BCD_COUNTER:
            PopulateRegisterProperties();
            break;
        case ComponentType::CLOCK_GENERATOR:
            PopulateClockProperties();
            break;
        case ComponentType::SEVEN_SEGMENT_DISPLAY:
        case ComponentType::LED_MATRIX_8X8:
            PopulateDisplayProperties();
            break;
        default:
            PopulateDefaultProperties();
            break;
    }
}

void PropertiesPanel::PopulateInputProperties() {
    // Input switch/pin properties - based on real input devices

    // Facing/Output direction
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Output Direction", "output_direction", facingChoices, wxArrayInt(), 0));

    // Input Device Type
    wxArrayString inputTypeChoices;
    inputTypeChoices.Add("Toggle Switch");
    inputTypeChoices.Add("Push Button (Momentary)");
    inputTypeChoices.Add("DIP Switch");
    inputTypeChoices.Add("Slide Switch");
    inputTypeChoices.Add("Rotary Switch");
    propGrid->Append(new wxEnumProperty("Input Type", "input_type", inputTypeChoices, wxArrayInt(), 0));

    // Bit Width for multi-bit inputs
    wxPGProperty* bitWidthProp = propGrid->Append(new wxIntProperty("Bit Width", "bit_width", 1));
    bitWidthProp->SetAttribute(wxPG_ATTR_MIN, 1);
    bitWidthProp->SetAttribute(wxPG_ATTR_MAX, 32);

    // Current State
    propGrid->Append(new wxBoolProperty("Current State", "current_state", false));

    // Default State (power-on state)
    wxArrayString defaultChoices;
    defaultChoices.Add("LOW (0)");
    defaultChoices.Add("HIGH (1)");
    defaultChoices.Add("High-Z");
    propGrid->Append(new wxEnumProperty("Default State", "default_state", defaultChoices, wxArrayInt(), 0));

    // Pull-up/Pull-down configuration
    wxArrayString pullChoices;
    pullChoices.Add("None (Floating)");
    pullChoices.Add("Pull-up");
    pullChoices.Add("Pull-down");
    pullChoices.Add("Keeper");
    propGrid->Append(new wxEnumProperty("Pull Configuration", "pull_config", pullChoices, wxArrayInt(), 1));

    // Debounce settings for mechanical switches
    propGrid->Append(new wxBoolProperty("Enable Debouncing", "enable_debounce", true));

    wxPGProperty* debounceProp = propGrid->Append(new wxFloatProperty("Debounce Time (ms)", "debounce_time", 20.0));
    debounceProp->SetAttribute(wxPG_ATTR_MIN, 1.0);
    debounceProp->SetAttribute(wxPG_ATTR_MAX, 1000.0);

    // Logic levels
    wxPGProperty* vihProp = propGrid->Append(new wxFloatProperty("VIH - Input High (V)", "vih", 3.5));
    vihProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    vihProp->SetAttribute(wxPG_ATTR_MAX, 15.0);

    wxPGProperty* vilProp = propGrid->Append(new wxFloatProperty("VIL - Input Low (V)", "vil", 1.5));
    vilProp->SetAttribute(wxPG_ATTR_MIN, 0.0);
    vilProp->SetAttribute(wxPG_ATTR_MAX, 5.0);

    // Current Output Value (read-only display)
    propGrid->Append(new wxStringProperty("Output Value", "output_value", "LOW"));
    propGrid->SetPropertyReadOnly("output_value");

    // Multi-bit value display (for bit_width > 1)
    propGrid->Append(new wxStringProperty("Binary Value", "binary_value", "0"));
    propGrid->SetPropertyReadOnly("binary_value");

    propGrid->Append(new wxStringProperty("Hex Value", "hex_value", "0x0"));
    propGrid->SetPropertyReadOnly("hex_value");

    // Component identification and display
    propGrid->Append(new wxStringProperty("Label", "label", ""));
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));

    // Switch appearance
    propGrid->Append(new wxColourProperty("Switch Color", "switch_color", wxColour(128, 128, 128)));
    propGrid->Append(new wxColourProperty("Active Color", "active_color", *wxGREEN));
}

void PropertiesPanel::PopulateOutputProperties() {
    // Output LED/indicator properties - based on real output devices

    // Input direction (where signal comes from)
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Input Direction", "input_direction", facingChoices, wxArrayInt(), 2)); // Default West

    // LED Type
    wxArrayString ledTypeChoices;
    ledTypeChoices.Add("Standard LED");
    ledTypeChoices.Add("High-Brightness LED");
    ledTypeChoices.Add("RGB LED");
    ledTypeChoices.Add("7-Segment Digit");
    ledTypeChoices.Add("Bar Graph");
    propGrid->Append(new wxEnumProperty("LED Type", "led_type", ledTypeChoices, wxArrayInt(), 0));

    // Bit Width for multi-bit displays
    wxPGProperty* bitWidthProp = propGrid->Append(new wxIntProperty("Bit Width", "bit_width", 1));
    bitWidthProp->SetAttribute(wxPG_ATTR_MIN, 1);
    bitWidthProp->SetAttribute(wxPG_ATTR_MAX, 32);

    // LED Electrical Properties
    wxPGProperty* forwardVoltageProp = propGrid->Append(new wxFloatProperty("Forward Voltage (V)", "forward_voltage", 2.1));
    forwardVoltageProp->SetAttribute(wxPG_ATTR_MIN, 1.0);
    forwardVoltageProp->SetAttribute(wxPG_ATTR_MAX, 5.0);

    wxPGProperty* forwardCurrentProp = propGrid->Append(new wxFloatProperty("Forward Current (mA)", "forward_current", 20.0));
    forwardCurrentProp->SetAttribute(wxPG_ATTR_MIN, 1.0);
    forwardCurrentProp->SetAttribute(wxPG_ATTR_MAX, 100.0);

    // Logic Threshold Levels
    wxPGProperty* vihProp = propGrid->Append(new wxFloatProperty("VIH - Input High (V)", "vih", 2.0));
    vihProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    vihProp->SetAttribute(wxPG_ATTR_MAX, 15.0);

    wxPGProperty* vilProp = propGrid->Append(new wxFloatProperty("VIL - Input Low (V)", "vil", 0.8));
    vilProp->SetAttribute(wxPG_ATTR_MIN, 0.0);
    vilProp->SetAttribute(wxPG_ATTR_MAX, 5.0);

    // Brightness Control
    wxPGProperty* brightnessProp = propGrid->Append(new wxIntProperty("Brightness (%)", "brightness", 100));
    brightnessProp->SetAttribute(wxPG_ATTR_MIN, 1);
    brightnessProp->SetAttribute(wxPG_ATTR_MAX, 100);

    // LED Colors
    propGrid->Append(new wxColourProperty("On Color", "on_color", *wxRED));
    propGrid->Append(new wxColourProperty("Off Color", "off_color", wxColour(64, 32, 32))); // Dark red

    // Blinking Configuration
    propGrid->Append(new wxBoolProperty("Enable Blinking", "enable_blink", false));

    wxPGProperty* blinkRateProp = propGrid->Append(new wxFloatProperty("Blink Rate (Hz)", "blink_rate", 2.0));
    blinkRateProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    blinkRateProp->SetAttribute(wxPG_ATTR_MAX, 50.0);

    // Current State Display (read-only)
    propGrid->Append(new wxBoolProperty("Current State", "current_state", false));
    propGrid->SetPropertyReadOnly("current_state");

    // Current Input Value
    wxString inputValue = "UNDEFINED";
    if (currentComponent) {
        const auto& pins = currentComponent->GetPins();
        for (const auto& pin : pins) {
            if (pin.isInput) {
                inputValue = pin.value == LogicValue::HIGH ? "HIGH" :
                            pin.value == LogicValue::LOW ? "LOW" : "UNDEFINED";
                break;
            }
        }
    }
    propGrid->Append(new wxStringProperty("Input Value", "input_value", inputValue));
    propGrid->SetPropertyReadOnly("input_value");

    // Multi-bit value display (for bit_width > 1)
    propGrid->Append(new wxStringProperty("Binary Value", "binary_value", "0"));
    propGrid->SetPropertyReadOnly("binary_value");

    propGrid->Append(new wxStringProperty("Hex Value", "hex_value", "0x0"));
    propGrid->SetPropertyReadOnly("hex_value");

    // Power consumption (calculated)
    propGrid->Append(new wxStringProperty("Power (mW)", "power_consumption", "0.0"));
    propGrid->SetPropertyReadOnly("power_consumption");

    // Component identification and display
    propGrid->Append(new wxStringProperty("Label", "label", ""));
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));
}

void PropertiesPanel::PopulateLogicGateProperties() {
    // Logic gate specific properties based on real electronic characteristics
    ComponentType type = currentComponent->GetType();

    // Facing direction - affects pin orientation
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Facing", "facing", facingChoices, wxArrayInt(), 0));

    // Input Count - varies by gate type
    int defaultInputs = (type == ComponentType::NOT_GATE || type == ComponentType::BUFFER) ? 1 : 2;
    int maxInputs = (type == ComponentType::NOT_GATE || type == ComponentType::BUFFER) ? 1 : 8;
    wxPGProperty* inputCountProp = propGrid->Append(new wxIntProperty("Input Count", "input_count", defaultInputs));
    inputCountProp->SetAttribute(wxPG_ATTR_MIN, 1);
    inputCountProp->SetAttribute(wxPG_ATTR_MAX, maxInputs);
    if (type == ComponentType::NOT_GATE || type == ComponentType::BUFFER) {
        propGrid->SetPropertyReadOnly("input_count"); // NOT gate always has 1 input
    }

    // Logic Family - affects electrical characteristics
    wxArrayString familyChoices;
    familyChoices.Add("TTL");
    familyChoices.Add("CMOS");
    familyChoices.Add("ECL");
    familyChoices.Add("NMOS");
    propGrid->Append(new wxEnumProperty("Logic Family", "logic_family", familyChoices, wxArrayInt(), 1)); // Default CMOS

    // Supply Voltage - affects logic levels
    wxArrayString voltageChoices;
    voltageChoices.Add("3.3V");
    voltageChoices.Add("5.0V");
    voltageChoices.Add("12V");
    voltageChoices.Add("15V");
    propGrid->Append(new wxEnumProperty("Supply Voltage", "supply_voltage", voltageChoices, wxArrayInt(), 1)); // Default 5V

    // Propagation Delay - critical timing parameter
    wxPGProperty* delayProp = propGrid->Append(new wxFloatProperty("Propagation Delay (ns)", "prop_delay", 5.0));
    delayProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    delayProp->SetAttribute(wxPG_ATTR_MAX, 1000.0);

    // Threshold Voltage - input switching point
    wxPGProperty* thresholdProp = propGrid->Append(new wxFloatProperty("Threshold Voltage (V)", "threshold_v", 2.5));
    thresholdProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    thresholdProp->SetAttribute(wxPG_ATTR_MAX, 10.0);

    // Fan-out - output driving capability
    wxPGProperty* fanoutProp = propGrid->Append(new wxIntProperty("Fan-out", "fanout", 10));
    fanoutProp->SetAttribute(wxPG_ATTR_MIN, 1);
    fanoutProp->SetAttribute(wxPG_ATTR_MAX, 50);

    // Current Output State (read-only)
    wxString outputValue = "UNDEFINED";
    if (currentComponent) {
        const auto& pins = currentComponent->GetPins();
        for (const auto& pin : pins) {
            if (!pin.isInput) {
                outputValue = pin.value == LogicValue::HIGH ? "HIGH" :
                             pin.value == LogicValue::LOW ? "LOW" : "UNDEFINED";
                break;
            }
        }
    }
    propGrid->Append(new wxStringProperty("Output State", "output_state", outputValue));
    propGrid->SetPropertyReadOnly("output_state");

    // Enable timing simulation
    propGrid->Append(new wxBoolProperty("Enable Timing Simulation", "enable_timing", false));

    // Component label for identification
    propGrid->Append(new wxStringProperty("Label", "label", ""));

    // Label display font
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));
}

void PropertiesPanel::PopulateSequentialProperties() {
    // Sequential logic elements: Flip-flops and Latches
    ComponentType type = currentComponent->GetType();

    // Facing direction
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Facing", "facing", facingChoices, wxArrayInt(), 0));

    // Clock Edge Sensitivity (not applicable to latches)
    if (type != ComponentType::SR_LATCH) {
        wxArrayString edgeChoices;
        edgeChoices.Add("Rising Edge");
        edgeChoices.Add("Falling Edge");
        edgeChoices.Add("Both Edges");
        propGrid->Append(new wxEnumProperty("Clock Edge", "clock_edge", edgeChoices, wxArrayInt(), 0));
    }

    // Initial State of Q output
    wxArrayString initialChoices;
    initialChoices.Add("LOW (0)");
    initialChoices.Add("HIGH (1)");
    initialChoices.Add("Unknown (X)");
    propGrid->Append(new wxEnumProperty("Initial State (Q)", "initial_state", initialChoices, wxArrayInt(), 0));

    // Reset Type
    wxArrayString resetChoices;
    resetChoices.Add("None");
    resetChoices.Add("Synchronous Reset");
    resetChoices.Add("Asynchronous Reset");
    propGrid->Append(new wxEnumProperty("Reset Type", "reset_type", resetChoices, wxArrayInt(), 2));

    // Set Type (for components that support it)
    if (type == ComponentType::SR_LATCH || type == ComponentType::JK_FLIPFLOP) {
        wxArrayString setChoices;
        setChoices.Add("None");
        setChoices.Add("Synchronous Set");
        setChoices.Add("Asynchronous Set");
        propGrid->Append(new wxEnumProperty("Set Type", "set_type", setChoices, wxArrayInt(), 0));
    }

    // Timing characteristics
    wxPGProperty* setupProp = propGrid->Append(new wxFloatProperty("Setup Time (ns)", "setup_time", 2.0));
    setupProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    setupProp->SetAttribute(wxPG_ATTR_MAX, 100.0);

    wxPGProperty* holdProp = propGrid->Append(new wxFloatProperty("Hold Time (ns)", "hold_time", 1.0));
    holdProp->SetAttribute(wxPG_ATTR_MIN, 0.0);
    holdProp->SetAttribute(wxPG_ATTR_MAX, 100.0);

    wxPGProperty* maxFreqProp = propGrid->Append(new wxFloatProperty("Max Clock Freq (MHz)", "max_freq", 100.0));
    maxFreqProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    maxFreqProp->SetAttribute(wxPG_ATTR_MAX, 1000.0);

    // Current state display (read-only)
    propGrid->Append(new wxStringProperty("Current Q State", "q_state", "UNDEFINED"));
    propGrid->SetPropertyReadOnly("q_state");

    propGrid->Append(new wxStringProperty("Current Q̄ State", "qbar_state", "UNDEFINED"));
    propGrid->SetPropertyReadOnly("qbar_state");

    // Enable metastability simulation
    propGrid->Append(new wxBoolProperty("Enable Metastability", "enable_metastability", false));

    // Component identification
    propGrid->Append(new wxStringProperty("Label", "label", ""));
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));
}

void PropertiesPanel::PopulateRegisterProperties() {
    // Register and Counter properties
    ComponentType type = currentComponent->GetType();

    // Facing direction
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Facing", "facing", facingChoices, wxArrayInt(), 0));

    // Bit Width
    int defaultWidth = (type == ComponentType::REGISTER_4BIT ||
                       type == ComponentType::SHIFT_REGISTER_4BIT ||
                       type == ComponentType::COUNTER_4BIT) ? 4 : 4;
    wxPGProperty* widthProp = propGrid->Append(new wxIntProperty("Bit Width", "bit_width", defaultWidth));
    widthProp->SetAttribute(wxPG_ATTR_MIN, 1);
    widthProp->SetAttribute(wxPG_ATTR_MAX, 32);

    // Clock Edge
    wxArrayString edgeChoices;
    edgeChoices.Add("Rising Edge");
    edgeChoices.Add("Falling Edge");
    propGrid->Append(new wxEnumProperty("Clock Edge", "clock_edge", edgeChoices, wxArrayInt(), 0));

    // Counter specific properties
    if (type == ComponentType::COUNTER_4BIT || type == ComponentType::BCD_COUNTER) {
        // Count Direction
        wxArrayString dirChoices;
        dirChoices.Add("Up");
        dirChoices.Add("Down");
        dirChoices.Add("Up/Down");
        propGrid->Append(new wxEnumProperty("Count Direction", "count_dir", dirChoices, wxArrayInt(), 0));

        // Maximum Count Value
        int maxCount = (type == ComponentType::BCD_COUNTER) ? 9 : 15;
        wxPGProperty* maxCountProp = propGrid->Append(new wxIntProperty("Max Count", "max_count", maxCount));
        maxCountProp->SetAttribute(wxPG_ATTR_MIN, 1);
        maxCountProp->SetAttribute(wxPG_ATTR_MAX, (type == ComponentType::BCD_COUNTER) ? 9 : 255);

        // Auto Reset
        propGrid->Append(new wxBoolProperty("Auto Reset", "auto_reset", true));
    }

    // Shift Register specific properties
    if (type == ComponentType::SHIFT_REGISTER_4BIT) {
        // Shift Direction
        wxArrayString shiftChoices;
        shiftChoices.Add("Left");
        shiftChoices.Add("Right");
        shiftChoices.Add("Bidirectional");
        propGrid->Append(new wxEnumProperty("Shift Direction", "shift_dir", shiftChoices, wxArrayInt(), 1));

        // Shift Type
        wxArrayString typeChoices;
        typeChoices.Add("Logical");
        typeChoices.Add("Arithmetic");
        typeChoices.Add("Circular");
        propGrid->Append(new wxEnumProperty("Shift Type", "shift_type", typeChoices, wxArrayInt(), 0));
    }

    // Common timing properties
    wxPGProperty* setupProp = propGrid->Append(new wxFloatProperty("Setup Time (ns)", "setup_time", 3.0));
    setupProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    setupProp->SetAttribute(wxPG_ATTR_MAX, 100.0);

    wxPGProperty* maxFreqProp = propGrid->Append(new wxFloatProperty("Max Clock Freq (MHz)", "max_freq", 50.0));
    maxFreqProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    maxFreqProp->SetAttribute(wxPG_ATTR_MAX, 500.0);

    // Current Value Display
    propGrid->Append(new wxStringProperty("Current Value", "current_value", "0000"));
    propGrid->SetPropertyReadOnly("current_value");

    // Enable features
    propGrid->Append(new wxBoolProperty("Enable Clock", "enable_clock", true));
    propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_pin_labels", true));

    // Component identification
    propGrid->Append(new wxStringProperty("Label", "label", ""));
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));
}

void PropertiesPanel::PopulateClockProperties() {
    // Clock generator properties - enhanced for real characteristics

    // Frequency setting with realistic range
    wxPGProperty* freqProp = propGrid->Append(new wxFloatProperty("Frequency (Hz)", "frequency", 1.0));
    freqProp->SetAttribute(wxPG_ATTR_MIN, 0.001);  // 1 mHz
    freqProp->SetAttribute(wxPG_ATTR_MAX, 1000000000.0);  // 1 GHz

    // Duty Cycle with precise control
    wxPGProperty* dutyProp = propGrid->Append(new wxFloatProperty("Duty Cycle (%)", "duty_cycle", 50.0));
    dutyProp->SetAttribute(wxPG_ATTR_MIN, 1.0);
    dutyProp->SetAttribute(wxPG_ATTR_MAX, 99.0);

    // Phase offset
    wxPGProperty* phaseProp = propGrid->Append(new wxFloatProperty("Phase Offset (°)", "phase_offset", 0.0));
    phaseProp->SetAttribute(wxPG_ATTR_MIN, 0.0);
    phaseProp->SetAttribute(wxPG_ATTR_MAX, 360.0);

    // Output amplitude
    wxPGProperty* amplitudeProp = propGrid->Append(new wxFloatProperty("Output Amplitude (V)", "amplitude", 5.0));
    amplitudeProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    amplitudeProp->SetAttribute(wxPG_ATTR_MAX, 15.0);

    // Rise and fall times
    wxPGProperty* riseProp = propGrid->Append(new wxFloatProperty("Rise Time (ns)", "rise_time", 1.0));
    riseProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    riseProp->SetAttribute(wxPG_ATTR_MAX, 1000.0);

    wxPGProperty* fallProp = propGrid->Append(new wxFloatProperty("Fall Time (ns)", "fall_time", 1.0));
    fallProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    fallProp->SetAttribute(wxPG_ATTR_MAX, 1000.0);

    // Clock jitter
    wxPGProperty* jitterProp = propGrid->Append(new wxFloatProperty("Jitter (ps)", "jitter", 0.0));
    jitterProp->SetAttribute(wxPG_ATTR_MIN, 0.0);
    jitterProp->SetAttribute(wxPG_ATTR_MAX, 10000.0);

    // Initial state
    wxArrayString initialChoices;
    initialChoices.Add("LOW");
    initialChoices.Add("HIGH");
    propGrid->Append(new wxEnumProperty("Initial State", "initial_state", initialChoices, wxArrayInt(), 0));

    // Control states
    propGrid->Append(new wxBoolProperty("Running", "clock_running", false));
    propGrid->Append(new wxBoolProperty("Enable Jitter", "enable_jitter", false));

    // Current state display
    propGrid->Append(new wxStringProperty("Current State", "current_clock_state", "LOW"));
    propGrid->SetPropertyReadOnly("current_clock_state");

    // Component identification
    propGrid->Append(new wxStringProperty("Label", "label", "CLK"));
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));
}

void PropertiesPanel::PopulateArithmeticProperties() {
    // Arithmetic components: Adders and related circuits
    ComponentType type = currentComponent->GetType();

    // Facing direction
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Facing", "facing", facingChoices, wxArrayInt(), 0));

    // Bit Width - critical for arithmetic operations
    int defaultWidth = (type == ComponentType::ADDER_4BIT) ? 4 : 1;
    wxPGProperty* widthProp = propGrid->Append(new wxIntProperty("Bit Width", "bit_width", defaultWidth));
    widthProp->SetAttribute(wxPG_ATTR_MIN, 1);
    widthProp->SetAttribute(wxPG_ATTR_MAX, 64);
    if (type == ComponentType::HALF_ADDER || type == ComponentType::FULL_ADDER) {
        propGrid->SetPropertyReadOnly("bit_width"); // Single bit adders
    }

    // Arithmetic Type
    wxArrayString arithChoices;
    arithChoices.Add("Unsigned");
    arithChoices.Add("Signed (2's Complement)");
    arithChoices.Add("BCD");
    propGrid->Append(new wxEnumProperty("Arithmetic Type", "arith_type", arithChoices, wxArrayInt(), 0));

    // Carry Chain Configuration
    if (type == ComponentType::ADDER_4BIT) {
        wxArrayString carryChoices;
        carryChoices.Add("Ripple Carry");
        carryChoices.Add("Carry Look-ahead");
        carryChoices.Add("Carry Select");
        propGrid->Append(new wxEnumProperty("Carry Type", "carry_type", carryChoices, wxArrayInt(), 0));

        // Carry Propagation Delay
        wxPGProperty* carryDelayProp = propGrid->Append(new wxFloatProperty("Carry Delay (ns)", "carry_delay", 5.0));
        carryDelayProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
        carryDelayProp->SetAttribute(wxPG_ATTR_MAX, 100.0);
    }

    // Sum/Carry Delays
    wxPGProperty* sumDelayProp = propGrid->Append(new wxFloatProperty("Sum Delay (ns)", "sum_delay", 8.0));
    sumDelayProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    sumDelayProp->SetAttribute(wxPG_ATTR_MAX, 100.0);

    // Overflow Detection
    propGrid->Append(new wxBoolProperty("Overflow Detection", "overflow_detect", false));

    // Current Values Display (read-only)
    if (type == ComponentType::HALF_ADDER) {
        propGrid->Append(new wxStringProperty("Sum Output", "sum_out", "0"));
        propGrid->SetPropertyReadOnly("sum_out");
        propGrid->Append(new wxStringProperty("Carry Output", "carry_out", "0"));
        propGrid->SetPropertyReadOnly("carry_out");
    } else if (type == ComponentType::FULL_ADDER) {
        propGrid->Append(new wxStringProperty("Sum Output", "sum_out", "0"));
        propGrid->SetPropertyReadOnly("sum_out");
        propGrid->Append(new wxStringProperty("Carry Output", "carry_out", "0"));
        propGrid->SetPropertyReadOnly("carry_out");
    } else if (type == ComponentType::ADDER_4BIT) {
        propGrid->Append(new wxStringProperty("Sum Output", "sum_out", "0000"));
        propGrid->SetPropertyReadOnly("sum_out");
        propGrid->Append(new wxStringProperty("Carry Output", "carry_out", "0"));
        propGrid->SetPropertyReadOnly("carry_out");
        propGrid->Append(new wxStringProperty("Overflow Flag", "overflow_flag", "0"));
        propGrid->SetPropertyReadOnly("overflow_flag");
    }

    // Pin Label Configuration
    propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_pin_labels", true));

    // Component identification
    propGrid->Append(new wxStringProperty("Label", "label", ""));
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));
}

void PropertiesPanel::PopulateMultiplexerProperties() {
    // Multiplexer and Demultiplexer components
    ComponentType type = currentComponent->GetType();

    // Facing direction
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Facing", "facing", facingChoices, wxArrayInt(), 0));

    // Channel Configuration
    int channelCount = 2;
    int selectBits = 1;
    if (type == ComponentType::MULTIPLEXER_4TO1 || type == ComponentType::DEMULTIPLEXER_1TO4) {
        channelCount = 4;
        selectBits = 2;
    }

    wxPGProperty* channelProp = propGrid->Append(new wxIntProperty("Channel Count", "channel_count", channelCount));
    propGrid->SetPropertyReadOnly("channel_count"); // Fixed by component type

    wxPGProperty* selectProp = propGrid->Append(new wxIntProperty("Select Bits", "select_bits", selectBits));
    propGrid->SetPropertyReadOnly("select_bits"); // Fixed by component type

    // Data Width per Channel
    wxPGProperty* dataWidthProp = propGrid->Append(new wxIntProperty("Data Width", "data_width", 1));
    dataWidthProp->SetAttribute(wxPG_ATTR_MIN, 1);
    dataWidthProp->SetAttribute(wxPG_ATTR_MAX, 32);

    // Enable Control
    propGrid->Append(new wxBoolProperty("Has Enable Input", "has_enable", true));

    // Enable Polarity
    wxArrayString enableChoices;
    enableChoices.Add("Active High");
    enableChoices.Add("Active Low");
    propGrid->Append(new wxEnumProperty("Enable Polarity", "enable_polarity", enableChoices, wxArrayInt(), 0));

    // Output Type
    wxArrayString outputChoices;
    outputChoices.Add("Push-Pull");
    outputChoices.Add("Open Drain");
    outputChoices.Add("Three-State");
    propGrid->Append(new wxEnumProperty("Output Type", "output_type", outputChoices, wxArrayInt(), 2));

    // Timing Properties
    wxPGProperty* propDelayProp = propGrid->Append(new wxFloatProperty("Propagation Delay (ns)", "prop_delay", 10.0));
    propDelayProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    propDelayProp->SetAttribute(wxPG_ATTR_MAX, 100.0);

    wxPGProperty* enableDelayProp = propGrid->Append(new wxFloatProperty("Enable Delay (ns)", "enable_delay", 15.0));
    enableDelayProp->SetAttribute(wxPG_ATTR_MIN, 0.1);
    enableDelayProp->SetAttribute(wxPG_ATTR_MAX, 100.0);

    // Current State Display
    if (type == ComponentType::MULTIPLEXER_2TO1 || type == ComponentType::MULTIPLEXER_4TO1) {
        propGrid->Append(new wxStringProperty("Selected Input", "selected_input", "0"));
        propGrid->SetPropertyReadOnly("selected_input");
        propGrid->Append(new wxStringProperty("Output Value", "output_value", "UNDEFINED"));
        propGrid->SetPropertyReadOnly("output_value");
    } else {
        propGrid->Append(new wxStringProperty("Selected Output", "selected_output", "0"));
        propGrid->SetPropertyReadOnly("selected_output");
        propGrid->Append(new wxStringProperty("Active Outputs", "active_outputs", "None"));
        propGrid->SetPropertyReadOnly("active_outputs");
    }

    // Pin Labeling
    propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_pin_labels", true));
    propGrid->Append(new wxBoolProperty("Show Channel Numbers", "show_channel_nums", true));

    // Component identification
    propGrid->Append(new wxStringProperty("Label", "label", ""));
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));
}

void PropertiesPanel::PopulateDisplayProperties() {
    // Display components: 7-segment displays, LED matrices
    ComponentType type = currentComponent->GetType();

    // Facing direction
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Facing", "facing", facingChoices, wxArrayInt(), 0));

    if (type == ComponentType::SEVEN_SEGMENT_DISPLAY) {
        // 7-Segment Display Properties

        // Common Cathode/Anode
        wxArrayString commonChoices;
        commonChoices.Add("Common Cathode");
        commonChoices.Add("Common Anode");
        propGrid->Append(new wxEnumProperty("Display Type", "display_type", commonChoices, wxArrayInt(), 0));

        // Decimal Point
        propGrid->Append(new wxBoolProperty("Has Decimal Point", "has_decimal", true));

        // BCD/7-Segment Input
        wxArrayString inputChoices;
        inputChoices.Add("BCD Input (4-bit)");
        inputChoices.Add("7-Segment Direct");
        propGrid->Append(new wxEnumProperty("Input Type", "input_type", inputChoices, wxArrayInt(), 0));

        // Segment Colors
        propGrid->Append(new wxColourProperty("On Color", "segment_on_color", *wxRED));
        propGrid->Append(new wxColourProperty("Off Color", "segment_off_color", wxColour(64, 64, 64)));

        // Brightness Control
        wxPGProperty* brightnessProp = propGrid->Append(new wxIntProperty("Brightness (%)", "brightness", 100));
        brightnessProp->SetAttribute(wxPG_ATTR_MIN, 1);
        brightnessProp->SetAttribute(wxPG_ATTR_MAX, 100);

        // Current Display
        propGrid->Append(new wxStringProperty("Current Digit", "current_digit", "0"));
        propGrid->SetPropertyReadOnly("current_digit");

    } else if (type == ComponentType::LED_MATRIX_8X8) {
        // LED Matrix Properties

        // Matrix Size
        wxPGProperty* rowsProp = propGrid->Append(new wxIntProperty("Rows", "matrix_rows", 8));
        rowsProp->SetAttribute(wxPG_ATTR_MIN, 1);
        rowsProp->SetAttribute(wxPG_ATTR_MAX, 32);

        wxPGProperty* colsProp = propGrid->Append(new wxIntProperty("Columns", "matrix_cols", 8));
        colsProp->SetAttribute(wxPG_ATTR_MIN, 1);
        colsProp->SetAttribute(wxPG_ATTR_MAX, 32);

        // Scan Type
        wxArrayString scanChoices;
        scanChoices.Add("Row Scan");
        scanChoices.Add("Column Scan");
        scanChoices.Add("No Multiplexing");
        propGrid->Append(new wxEnumProperty("Scan Type", "scan_type", scanChoices, wxArrayInt(), 0));

        // Scan Rate
        wxPGProperty* scanRateProp = propGrid->Append(new wxFloatProperty("Scan Rate (Hz)", "scan_rate", 100.0));
        scanRateProp->SetAttribute(wxPG_ATTR_MIN, 1.0);
        scanRateProp->SetAttribute(wxPG_ATTR_MAX, 10000.0);

        // LED Properties
        propGrid->Append(new wxColourProperty("LED On Color", "led_on_color", *wxRED));
        propGrid->Append(new wxColourProperty("LED Off Color", "led_off_color", wxColour(32, 32, 32)));

        // Brightness Control
        wxPGProperty* brightnessProp = propGrid->Append(new wxIntProperty("Brightness (%)", "brightness", 100));
        brightnessProp->SetAttribute(wxPG_ATTR_MIN, 1);
        brightnessProp->SetAttribute(wxPG_ATTR_MAX, 100);
    }

    // Component identification
    propGrid->Append(new wxStringProperty("Label", "label", ""));
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));
}

void PropertiesPanel::PopulateDefaultProperties() {
    // Default properties for other components with requested attributes

    // Facing direction
    wxArrayString facingChoices;
    facingChoices.Add("East");
    facingChoices.Add("North");
    facingChoices.Add("West");
    facingChoices.Add("South");
    propGrid->Append(new wxEnumProperty("Facing", "facing", facingChoices, wxArrayInt(), 0));

    // Data Bits (for complex components, this might be higher)
    int defaultDataBits = 1;
    if (currentComponent) {
        ComponentType type = currentComponent->GetType();
        if (type == ComponentType::ADDER_4BIT) defaultDataBits = 4;
        else if (type == ComponentType::MULTIPLEXER_4TO1 || type == ComponentType::DEMULTIPLEXER_1TO4) defaultDataBits = 2;
    }
    propGrid->Append(new wxIntProperty("Data Bits", "data_bits", defaultDataBits));

    // Gate Size (for components that have variable sizes)
    wxArrayString sizeChoices;
    sizeChoices.Add("Narrow");
    sizeChoices.Add("Normal");
    sizeChoices.Add("Wide");
    propGrid->Append(new wxEnumProperty("Gate Size", "gate_size", sizeChoices, wxArrayInt(), 1));

    // Output Value (shows current state)
    wxString outputValue = "UNDEFINED";
    if (currentComponent) {
        const auto& pins = currentComponent->GetPins();
        for (const auto& pin : pins) {
            if (!pin.isInput) {
                outputValue = pin.value == LogicValue::HIGH ? "HIGH" :
                             pin.value == LogicValue::LOW ? "LOW" : "UNDEFINED";
                break;
            }
        }
    }
    propGrid->Append(new wxStringProperty("Output Value", "output_value", outputValue));
    propGrid->SetPropertyReadOnly("output_value");

    // Label
    propGrid->Append(new wxStringProperty("Label", "label", ""));

    // Label Font
    wxFont defaultFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    propGrid->Append(new wxFontProperty("Label Font", "label_font", defaultFont));

    // Description
    propGrid->Append(new wxStringProperty("Description", "description", "Standard component"));
}

void PropertiesPanel::PopulatePinProperties() {
    wxPGProperty* pinsCat = propGrid->Append(new wxPropertyCategory("Pins Information"));
    const auto& pins = currentComponent->GetPins();

    propGrid->Append(new wxIntProperty("Total Pins", "pin_count", pins.size()));
    propGrid->SetPropertyReadOnly("pin_count");

    int inputCount = 0, outputCount = 0;
    for (const auto& pin : pins) {
        if (pin.isInput) inputCount++;
        else outputCount++;
    }

    propGrid->Append(new wxIntProperty("Input Pins", "input_pins", inputCount));
    propGrid->Append(new wxIntProperty("Output Pins", "output_pins", outputCount));
    propGrid->SetPropertyReadOnly("input_pins");
    propGrid->SetPropertyReadOnly("output_pins");

    // Component-specific properties
    SetupComponentProperties();
}

void PropertiesPanel::SetupComponentProperties() {
    if (!currentComponent) return;

    wxPGProperty* specificCat = propGrid->Append(new wxPropertyCategory("Component Properties"));

    ComponentType type = currentComponent->GetType();

    // Add properties based on component type
    switch (type) {
        case ComponentType::INPUT_PIN: {
            propGrid->Append(new wxBoolProperty("State", "input_state", false));
            propGrid->Append(new wxStringProperty("Label", "input_label", "Input"));
            break;
        }
        case ComponentType::OUTPUT_PIN: {
            const auto& pins = currentComponent->GetPins();
            if (!pins.empty()) {
                wxString stateStr = pins[0].value == LogicValue::HIGH ? "HIGH" :
                                  pins[0].value == LogicValue::LOW ? "LOW" : "UNDEFINED";
                propGrid->Append(new wxStringProperty("Current State", "output_state", stateStr));
                propGrid->SetPropertyReadOnly("output_state");
            }
            propGrid->Append(new wxStringProperty("Label", "output_label", "Output"));
            break;
        }
        case ComponentType::ADDER_4BIT: {
            propGrid->Append(new wxStringProperty("Data Width", "data_width", "4 bits"));
            propGrid->SetPropertyReadOnly("data_width");
            propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_labels", true));
            break;
        }
        case ComponentType::MULTIPLEXER_2TO1:
        case ComponentType::MULTIPLEXER_4TO1: {
            int inputs = type == ComponentType::MULTIPLEXER_2TO1 ? 2 : 4;
            propGrid->Append(new wxIntProperty("Input Count", "mux_inputs", inputs));
            propGrid->SetPropertyReadOnly("mux_inputs");
            propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_labels", true));
            break;
        }
        case ComponentType::DEMULTIPLEXER_1TO2:
        case ComponentType::DEMULTIPLEXER_1TO4: {
            int outputs = type == ComponentType::DEMULTIPLEXER_1TO2 ? 2 : 4;
            propGrid->Append(new wxIntProperty("Output Count", "demux_outputs", outputs));
            propGrid->SetPropertyReadOnly("demux_outputs");
            propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_labels", true));
            break;
        }
        default: {
            // Generic logic gate properties
            propGrid->Append(new wxStringProperty("Logic Function", "logic_function",
                type == ComponentType::AND_GATE ? "AND" :
                type == ComponentType::OR_GATE ? "OR" :
                type == ComponentType::NOT_GATE ? "NOT" :
                type == ComponentType::NAND_GATE ? "NAND" :
                type == ComponentType::NOR_GATE ? "NOR" :
                type == ComponentType::XOR_GATE ? "XOR" :
                type == ComponentType::XNOR_GATE ? "XNOR" : "Unknown"));
            propGrid->SetPropertyReadOnly("logic_function");

            if (type != ComponentType::NOT_GATE) {
                propGrid->Append(new wxIntProperty("Input Count", "input_count", 2));
                propGrid->SetPropertyReadOnly("input_count");
            }
            break;
        }
    }

    // Appearance properties
    wxPGProperty* appearanceCat = propGrid->Append(new wxPropertyCategory("Appearance"));
    propGrid->Append(new wxBoolProperty("Selected", "is_selected", currentComponent->IsSelected()));
    propGrid->SetPropertyReadOnly("is_selected");

    propGrid->Append(new wxColourProperty("Border Color", "border_color",
                      currentComponent->IsSelected() ? *wxRED : *wxBLACK));
    propGrid->Append(new wxColourProperty("Fill Color", "fill_color", wxColour(240, 240, 240)));
}

void PropertiesPanel::OnPropertyChanged(wxPropertyGridEvent& event) {
    if (!currentComponent) return;

    wxPGProperty* prop = event.GetProperty();
    if (!prop) return;

    wxString name = prop->GetName();

    // Handle position changes
    if (name == "pos_x" || name == "pos_y") {
        wxPoint currentPos = currentComponent->GetPosition();
        int newX = propGrid->GetPropertyValue("pos_x").GetLong();
        int newY = propGrid->GetPropertyValue("pos_y").GetLong();

        wxPoint offset(newX - currentPos.x, newY - currentPos.y);
        if (offset.x != 0 || offset.y != 0) {
            currentComponent->Move(offset);
            GetParent()->Refresh(); // Refresh the canvas
        }
    }

    // Handle rotation changes
    if (name == "rotation") {
        double newRotation = propGrid->GetPropertyValue("rotation").GetLong();
        currentComponent->SetRotation(newRotation);
        GetParent()->Refresh();
    }

    // Handle scale changes
    if (name == "scale_x" || name == "scale_y") {
        double scaleX = propGrid->GetPropertyValue("scale_x").GetDouble();
        double scaleY = propGrid->GetPropertyValue("scale_y").GetDouble();
        currentComponent->SetScale(scaleX, scaleY);
        GetParent()->Refresh();
    }

    // Handle other property changes as needed
    // This can be extended for more interactive properties
}

void PropertiesPanel::UpdateProperties() {
    if (currentComponent) {
        PopulateProperties();
    }
}

wxString PropertiesPanel::GetComponentName() const {
    if (!currentComponent) return "Unknown";

    ComponentType type = currentComponent->GetType();
    switch (type) {
        case ComponentType::AND_GATE: return "AND Gate";
        case ComponentType::OR_GATE: return "OR Gate";
        case ComponentType::NOT_GATE: return "NOT Gate";
        case ComponentType::NAND_GATE: return "NAND Gate";
        case ComponentType::NOR_GATE: return "NOR Gate";
        case ComponentType::XOR_GATE: return "XOR Gate";
        case ComponentType::XNOR_GATE: return "XNOR Gate";
        case ComponentType::INPUT_PIN: return "Input Switch";
        case ComponentType::OUTPUT_PIN: return "Output LED";
        case ComponentType::HALF_ADDER: return "Half Adder";
        case ComponentType::FULL_ADDER: return "Full Adder";
        case ComponentType::ADDER_4BIT: return "4-bit Adder";
        case ComponentType::MULTIPLEXER_2TO1: return "2:1 MUX";
        case ComponentType::MULTIPLEXER_4TO1: return "4:1 MUX";
        case ComponentType::DEMULTIPLEXER_1TO2: return "1:2 DEMUX";
        case ComponentType::DEMULTIPLEXER_1TO4: return "1:4 DEMUX";
        case ComponentType::D_FLIPFLOP: return "D Flip-Flop";
        case ComponentType::JK_FLIPFLOP: return "JK Flip-Flop";
        case ComponentType::SR_LATCH: return "SR Latch";
        case ComponentType::CLOCK_GENERATOR: return "Clock Generator";
        case ComponentType::SEVEN_SEGMENT_DISPLAY: return "7-Segment Display";
        case ComponentType::LED_MATRIX_8X8: return "LED Matrix 8x8";
        case ComponentType::DECODER_3TO8: return "3:8 Decoder";
        case ComponentType::BCD_TO_7SEGMENT: return "BCD to 7-Segment";
        case ComponentType::PRIORITY_ENCODER: return "Priority Encoder";
        default: return "Component";
    }
}